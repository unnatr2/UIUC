#ifndef BAR_H
#define BAR_H

#include <string>
#include "Foo.h"

using namespace std;

namespace potd {
    class Bar {
        public:
            Bar(string);
            Bar(const Bar &);
            Bar & operator=(const Bar &);
            ~Bar();
            string get_name();
        private:
            void _copy(const Bar & other);
            void _destroy();
            Foo *f_;
    };
}

#endif
