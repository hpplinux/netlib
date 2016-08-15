/*
 * Log.h
 *
 *  Created on: Dec 25,2015
 *      Author: zhangyalei
 */

#ifndef LOG_H_
#define LOG_H_

#include <cstdarg>
#include <string>
#include <vector>
#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Public_Define.h"

class Log {
public:
	typedef boost::unordered_map<long, Msg_Process_Time> Msg_Process_Time_Map;
	typedef std::vector<Msg_Process_Time> Msg_Process_Time_Vec;

	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];

	static Log *instance(void);

	void log_trace(const char *fmt, ...);
	void log_debug(const char *fmt, ...);
	void log_info(const char *fmt, ...);
	void log_warn(const char *fmt, ...);
	void log_error(const char *fmt, ...);
	void log_fatal(const char *fmt, ...);

	void set_log_type(int type, int sub_type = 0);
	void set_file_switcher(bool file_switcher);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream);
	int logging_db(Block_Buffer &buf);

	void free_cache(void);

	void msg_time(int msg_id, Time_Value &time);
	void show_msg_time(Time_Value &now);
	void show_msg_time(void);

private:
	Log(void);
	virtual ~Log(void);

private:
	static Log *instance_;
	int log_type_;						//日志类型
	int log_sub_type_;				//日志子类型
	bool file_switcher_;			//是否写入文件
	Time_Value show_time_;
	Msg_Process_Time_Map msg_time_;
};

////////////////////////////////////////////////////////////////////////////////

inline void Log::set_log_type(int type, int sub_type) {
	log_type_ = type;
	log_sub_type_ = sub_type;
}

inline void Log::set_file_switcher(bool file_switcher) {
	file_switcher_ = file_switcher;
}

//打印程序运行堆栈,跟踪记录数据信息
#define LOG_TRACE(FMT, ...) do {						\
		Log::instance()->log_trace("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//调试信息
#define LOG_DEBUG(FMT, ...) do {					\
		Log::instance()->log_debug("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//突出强调应用程序的运行过程
#define LOG_INFO(FMT, ...) do {						\
		Log::instance()->log_info("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//出现潜在错误的情形
#define LOG_WARN(FMT, ...) do {						\
		Log::instance()->log_warn("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//虽然发生错误事件，但仍然不影响系统的继续运行
#define LOG_ERROR(FMT, ...) do {						\
		Log::instance()->log_error("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//严重的错误事件，将会导致应用程序的退出
#define LOG_FATAL(FMT, ...) do {					\
		Log::instance()->log_fatal("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)


class Perf_Mon {
public:
	Perf_Mon(int msg_id)
		: msg_id_(msg_id), tv_(Time_Value::gettimeofday()) { }

	~Perf_Mon(void) {
		Time_Value res_tv = Time_Value::gettimeofday() - tv_;
		Log::instance()->msg_time(msg_id_, res_tv);
	}

private:
	int msg_id_;
	Time_Value tv_;
};

#endif /* LOG_H_ */
