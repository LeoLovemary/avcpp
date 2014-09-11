#ifndef AV_CODECCONTEXT_H
#define AV_CODECCONTEXT_H

#include "ffmpeg.h"
#include "stream.h"

namespace av {

class CodecContext : public FFWrapperPtr<AVCodecContext>
{
private:
    explicit CodecContext(const Stream2 &st);

public:
    ~CodecContext();

    // Disable copy/move
    CodecContext(const CodecContext&) = delete;
    void operator=(const CodecContext&) = delete;

    // Common
    void setCodec(const Codec &codec);

    bool open();
    bool close();
    bool isOpened() const { return m_isOpened; }
    bool isValid() const;

    /**
     * Copy codec context from codec context associated with given stream or other codec context.
     * This functionality useful for remuxing without deconding/encoding. In this case you need not
     * open codecs, only copy context.
     *
     * @param other  stream or codec context
     * @return true if context copied, false otherwise
     */
    /// @{
    bool copyContextFrom(const CodecContext &other);
    /// @}

    Rational timeBase() const;
    void setTimeBase(const Rational &value);

    const Stream2& stream() const;

    AVMediaType codecType() const;

    void        setOption(const std::string &key, const std::string &val, int flags = 0);

    bool        isAudio() const;
    bool        isVideo() const;

    // Common
    int                 frameSize() const;
    int                 frameNumber() const;

    // Video
    int                 width() const;
    int                 height() const;
    int                 codedWidth() const;
    int                 codedHeight() const;
    AVPixelFormat       pixelFormat() const;
    Rational            frameRate();
    int32_t             bitRate() const;
    std::pair<int, int> bitRateRange() const;
    int32_t             globalQuality();
    int32_t             gopSize();
    int                 bitRateTolerance() const;
    int                 strict() const;
    int                 maxBFrames() const;

    void setWidth(int w); // Note, it also sets coded_width
    void setHeight(int h); // Note, it also sets coded_height
    void setCodedWidth(int w);
    void setCodedHeight(int h);
    void setPixelFormat(AVPixelFormat pixelFormat);
    void setFrameRate(const Rational &frameRate);
    void setBitRate(int32_t bitRate);
    void setBitRateRange(const std::pair<int, int> &bitRateRange);
    void setGlobalQuality(int32_t quality);
    void setGopSize(int32_t size);
    void setBitRateTolerance(int bitRateTolerance);
    void setStrict(int strict);
    void setMaxBFrames(int maxBFrames);

    // Audio
    int            sampleRate() const;
    int            channels() const;
    AVSampleFormat sampleFormat() const;
    uint64_t       channelLayout() const;

    void        setSampleRate(int sampleRate);
    void        setChannels(int channels);
    void        setSampleFormat(AVSampleFormat sampleFormat);
    void        setChannelLayout(uint64_t layout);

    // Flags
    /// Access to CODEC_FLAG_* flags
    /// @{
    void        setFlags(int flags);
    void        addFlags(int flags);
    void        clearFlags(int flags);
    int         getFlags();
    bool        isFlags(int flags);
    /// @}

    // Flags 2
    /// Access to CODEC_FLAG2_* flags
    /// @{
    void        setFlags2(int flags2);
    void        addFlags2(int flags2);
    void        clearFlags2(int flags2);
    int         getFlags2();
    bool        isFlags2(int flags2);
    /// @}

    // Video
//    ssize_t decodeVideo(const FramePtr  &outFrame,  const PacketPtr &inPacket, size_t offset = 0);
//    ssize_t encodeVideo(const VideoFramePtr &inFrame,
//                        const EncodedPacketHandler &onPacketHandler);

    // Audio
//    ssize_t decodeAudio(const FramePtr &outFrame, const PacketPtr &inPacket, size_t offset = 0);
//    ssize_t encodeAudio(const FramePtr  &inFrame,
//                        const EncodedPacketHandler &onPacketHandler);

    // Video
    //ssize_t decode();

    bool    isValidForEncode();

private:
    ssize_t decodeCommon(AVFrame *outFrame, const Packet &inPacket, size_t offset,
                         int (*decodeProc)(AVCodecContext*, AVFrame*,int *, const AVPacket *));

//    ssize_t encodeCommon(const FramePtr  &inFrame,
//                         int (*encodeProc)(AVCodecContext*, AVPacket*,const AVFrame*, int*),
//                         const EncodedPacketHandler &onPacketHandler);

    void warnIfNotVideo() const;
    void warnIfNotAudio() const;

private:
    Direction       m_direction;
    Rational        m_fakePtsTimeBase;
    int64_t         m_fakeNextPts;
    int64_t         m_fakeCurrPts;

    Stream2         m_stream;
    Codec           m_codec;
    bool            m_isOpened = false;
};

} // namespace av

#endif // AV_CODECCONTEXT_H
