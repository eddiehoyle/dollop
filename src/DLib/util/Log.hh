#ifndef DOLLOP_LOG_HH
#define DOLLOP_LOG_HH

#include <mutex>
#include <ostream>

namespace dollop {

enum class Level {
    Debug,
    Info,
    Warning,
    Error,
};

class Stream;
class NullStream;

class Log {
public:
    Log( std::ostream* stream, Level level );
private:
    std::mutex m_mutex;
    std::ostream* m_stream;
    Level m_level;

    friend Stream;
    friend NullStream;
};

class Stream {
public:
    explicit Stream( Log& log, const std::string& prefix  );
    ~Stream();

    template< typename T >
    Stream& operator<<( T value );

private:
    Stream( Stream& ) = delete;
    Stream& operator=( Stream& ) = delete;
    Stream& operator=( Stream&& ) = delete;

    Log& m_log;
};

template< typename T >
Stream& Stream::operator<<( T value ) {
    *( m_log.m_stream ) << value;
    return *this;
}

class NullStream {
public:
    explicit NullStream( Log& log, const std::string& prefix );
    ~NullStream() = default;

    template< typename T >
    NullStream& operator<<( T value );

private:
    NullStream( NullStream& ) = delete;
    NullStream& operator=( Stream& ) = delete;
    NullStream& operator=( NullStream&& ) = delete;

    Log& m_log;
};

template< typename T >
NullStream& NullStream::operator<<( T value ) {
    return *this;
}

Log& debug_log();
Log& info_log();
Log& warn_log();
Log& error_log();

} // namespace dollop

#define DLP_INITIALISE_LOG() \
    ::dollop::debug_log();   \
    ::dollop::info_log();    \
    ::dollop::warn_log();    \
    ::dollop::error_log();

#ifdef DLP_LOGGING_ENABLED
#define DLP_DEBUG ::dollop::Stream( ::dollop::debug_log(), "DBG" ) << "[" << __FUNCTION__ << "]: "
#define DLP_INFO ::dollop::Stream( ::dollop::info_log(), "NFO" ) << "[" << __FUNCTION__ << "]: "
#define DLP_WARN ::dollop::Stream( ::dollop::warn_log(), "WRN" ) << "[" << __FUNCTION__ << "]: "
#define DLP_ERROR ::dollop::Stream( ::dollop::error_log(), "ERR" ) << "[" << __FUNCTION__ << "]: "
#else
#define DLP_DEBUG ::dollop::NullStream( ::dollop::debug_log(), "DBG" )
#define DLP_INFO ::dollop::NullStream( ::dollop::info_log(), "NFO" )
#define DLP_WARN ::dollop::NullStream( ::dollop::warn_log(), "WRN" )
#define DLP_ERROR ::dollop::NullStream( ::dollop::error_log(), "ERR" )
#endif

#endif // DOLLOP_LOG_HH
