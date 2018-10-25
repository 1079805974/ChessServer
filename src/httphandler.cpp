#include "httphandler.h"

using namespace uWS;
using namespace std;

void httpHandler(HttpResponse *res, HttpRequest req, char *data,
                 size_t length, size_t remainingBytes)
{
    char buffer[1024];
    int size = 0;
    cout << req.getUrl().toString() << endl;
    string trueFile = "../webapp" + req.getUrl().toString();
    int fd = open(trueFile.c_str(), O_RDONLY);
    if (fd == -1)
    {
        string s = "404";
        res->end(s.data(), s.length());
    }
    else
    {
        stringstream ss;
        struct stat statbuf;
        stat(trueFile.c_str(), &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            string s = "404";
            res->end(s.data(), s.length());
            return;
        }
        int size = statbuf.st_size;
        ss << size;
        res->write("HTTP 1.1 200 OK\r\n", 17);
        string contentLength = ("Content-length:" + ss.str() + "\r\n");
        res->write(contentLength.c_str(), contentLength.length());
        string suffixStr = trueFile.substr(trueFile.find_last_of('.'));
        auto it = contentTypeMap.find(suffixStr);
        if (it != contentTypeMap.end()){
            string contentType = "Content-type: " + it->second + "\r\n";
            res->write(contentType.c_str(), contentType.length());
        }
        res->write("\r\n", 2);
        while ((size = read(fd, buffer, 1024)) != 0)
        {
            res->write(buffer, size);
        }
        res->end();
        close(fd);
    }
}

