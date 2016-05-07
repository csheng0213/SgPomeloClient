#ifndef MD5_H__
#define MD5_H__

#include <string>

class MD5
{
public:
	MD5();
	virtual ~MD5();
	void feed(const unsigned char *data, int bytes);
	void feed(const std::string& data);
	void finalize();
	const std::string hex();
	const std::string binary();
	void reset();

private:
	struct MD5State
	{
		unsigned int count[2]; /* message length in bits, lsw first */
		unsigned int abcd[4]; /* digest buffer */
		unsigned char buf[64]; /* accumulate block */
	} m_state;
	void init();
	void process(const unsigned char *data);
	static const unsigned char pad[64];
	bool m_finished;
};

#endif // MD5_H__
