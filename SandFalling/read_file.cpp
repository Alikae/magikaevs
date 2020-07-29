//#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <stdio.h>

static unsigned int		calc_file_len(const char* file_name)
{
	char	buf[256];
	unsigned int		len;
	int		ret;
	int		fd;

	len = 0;
	if (_sopen_s(&fd, file_name, _O_RDONLY, _SH_DENYWR, _S_IREAD))
	{
		printf(":%i", errno);
		_write(2, "open ERROR\n", 11);
		return (0);
	}
	while ((ret = _read(fd, buf, 256)) > 0)
	{
		len += ret;
	}
	_close(fd);
	if (ret < 0)
	{
		printf(":%i", errno);
		_write(2, "read ERROR\n", 11);
		return (0);
	}
	return (len);
}

static int		copy_file_to_buf(const char* name, char* buf, int len)
{
	int	fd;

	if (_sopen_s(&fd, name, _O_RDONLY, _SH_DENYWR, _S_IREAD))
	{
		printf(":%i", errno);
		_write(2, "open ERROR\n", 11);
		return (0);
	}
	int ret;
	if ((ret = _read(fd, buf, len)) < 0)
	{
		printf(":%i/%i", ret, len);
		_write(2, "read ERROR\n", 11);
		return (0);
	}
	printf("BUG read_file :%i/%i\n", ret, len);
	buf[ret] = 0;
	_close(fd);
	return (1);
}

char* read_file(const char* file_name)
{
	unsigned int		len;
	char* file;

	if (!(len = calc_file_len(file_name)))
	{
		_write(2, "file error\n", 11);
		exit(2);
	}
	//printf("file of len %i reading\n", len);
	if (!(file = (char*)malloc(((unsigned long long)len + 1) * sizeof(char))))
	{
		_write(2, "malloc error\n", 13);
		exit(2);
	}
	file[len] = 0;
	//printf("file len: %i\n", len);
	if (!copy_file_to_buf(file_name, file, len))
	{
		free(file);
		exit(2);
	}
	//printf("Have read : "/*"%s"*/"\n", file);
	return (file);
}