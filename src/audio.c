void audioDataCallback(ma_device* sfx, void* output, const void* input, u32 sampleCount)
{
    NOT_USED(input);

    GameContext* context = (GameContext*)sfx->pUserData;
    if (!context)
    {
        return;
    }

    ma_uint64 read = ma_decoder_read_pcm_frames(&context->decoder, output, sampleCount);
    if (read == 0)
    {
        ma_decoder_seek_to_pcm_frame(&context->decoder, 0);
    }
}

bool initAudio(GameContext* context)
{
    ma_device_config sfxConfig;
    sfxConfig = ma_device_config_init(ma_device_type_playback);
    sfxConfig.playback.format = context->decoder.outputFormat;
    sfxConfig.playback.channels = context->decoder.outputChannels;
    sfxConfig.sampleRate = context->decoder.outputSampleRate;
    sfxConfig.dataCallback = audioDataCallback;
    sfxConfig.pUserData = context;

    if (ma_device_init(NULL, &sfxConfig, &context->sfx) != MA_SUCCESS)
    {
        logError("Failed to open playback device.\n");
        return false;
    }

    if (ma_device_start(&context->sfx) != MA_SUCCESS)
    {
        logError("Failed to start playback device.\n");
        ma_device_uninit(&context->sfx);
        return false;
    }

    return true;
}
