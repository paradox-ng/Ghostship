#include "AudioBankFactory.h"
#include "port/importer/types/AudioBank.h"
#include <bridge/resourcebridge.h>
#include "ResourceUtil.h"

// ADSR envelope delay/arg are consumed by the audio engine (audio/effects.h) through
// a byteswap that is a no-op on big-endian and a swap on little-endian, because the
// values are kept in N64 (big-endian) order. reader->ReadInt16() already returns the
// correct logical value (the resource loader sets the reader's byte order), so we must
// re-encode to that same engine convention. The bare BSWAP16 macro in scope here
// resolves to the UNCONDITIONAL __builtin_bswap16 from the binary-tools endianness
// header, which double-swaps on big-endian and corrupts every envelope (sustained
// music notes collapse, so the OST goes silent while short SFX survive). This matches
// the BSWAP16_BE helper the sequence factory uses for the same reason.
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ENV_TO_ENGINE(x) (x)
#else
#define ENV_TO_ENGINE(x) ((int16_t)__builtin_bswap16((uint16_t)(x)))
#endif

std::shared_ptr<Ship::IResource>
SM64::AudioBankFactoryV0::ReadResource(std::shared_ptr<Ship::File> file,
                                       std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    std::shared_ptr<AudioBank> bank = std::make_shared<AudioBank>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint8_t bankId = reader->ReadUInt32();
    uint32_t instrumentCount = reader->ReadUInt32();

    for (size_t i = 0; i < instrumentCount; i++) {
        auto* instrument = new Instrument();
        bool valid = reader->ReadUByte();
        if (!valid) {
            bank->instruments.push_back(nullptr);
            continue;
        }
        instrument->loaded = 1;
        instrument->releaseRate = reader->ReadUByte();
        instrument->normalRangeLo = reader->ReadUByte();
        instrument->normalRangeHi = reader->ReadUByte();

        uint32_t envelopeSize = reader->ReadUInt32();
        if (envelopeSize != 0) {
            instrument->envelope = new AdsrEnvelope[envelopeSize];
            for (size_t j = 0; j < envelopeSize; j++) {
                instrument->envelope[j].delay = ENV_TO_ENGINE(reader->ReadInt16());
                instrument->envelope[j].arg = ENV_TO_ENGINE(reader->ReadInt16());
            }
        }

        uint32_t soundFlags = reader->ReadUInt32();
        bool hasLo = soundFlags & (1 << 0);
        bool hasMed = soundFlags & (1 << 1);
        bool hasHi = soundFlags & (1 << 2);

        if (hasLo) {
            std::string lowSampleName = reader->ReadString();
            instrument->lowNotesSound.sample = LoadChild<AudioBankSample*>(lowSampleName.c_str());
            instrument->lowNotesSound.tuning = reader->ReadFloat();
        }

        if (hasMed) {
            std::string normalSampleName = reader->ReadString();
            instrument->normalNotesSound.sample = LoadChild<AudioBankSample*>(normalSampleName.c_str());
            instrument->normalNotesSound.tuning = reader->ReadFloat();
        }

        if (hasHi) {
            std::string highSampleName = reader->ReadString();
            instrument->highNotesSound.sample = LoadChild<AudioBankSample*>(highSampleName.c_str());
            instrument->highNotesSound.tuning = reader->ReadFloat();
        }

        bank->instruments.push_back(instrument);
    }

    uint32_t drumCount = reader->ReadUInt32();

    for (size_t i = 0; i < drumCount; i++) {
        auto* drum = new Drum();
        drum->releaseRate = reader->ReadUByte();
        drum->pan = reader->ReadUByte();
        drum->loaded = 1;

        uint32_t envelopeSize = reader->ReadUInt32();
        if (envelopeSize != 0) {
            drum->envelope = new AdsrEnvelope[envelopeSize];
            for (size_t j = 0; j < envelopeSize; j++) {
                drum->envelope[j].delay = ENV_TO_ENGINE(reader->ReadInt16());
                drum->envelope[j].arg = ENV_TO_ENGINE(reader->ReadInt16());
            }
        }

        std::string sampleName = reader->ReadString();
        drum->sound.sample = LoadChild<AudioBankSample*>(sampleName.c_str());
        drum->sound.tuning = reader->ReadFloat();

        bank->drums.push_back(drum);
    }

    bank->mData.bankId = bankId;
    bank->mData.numInstruments = instrumentCount;
    bank->mData.numDrums = drumCount;
    bank->mData.instruments = bank->instruments.data();
    bank->mData.drums = bank->drums.data();

    return bank;
}