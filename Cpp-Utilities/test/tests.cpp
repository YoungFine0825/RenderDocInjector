/*
    CONTRIBUTORS:
        Sean Pesce

*/
#include "CppUnitTest.h"

#include "sp/gui/console/console_message.h"
#include "sp/gui/console/console_output.h"
#include "sp/utility/history.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{        
    TEST_CLASS(ConsoleTest1)
    {
    public:

        TEST_METHOD(history_container)
        {
            using namespace sp::util;
            history<std::string> h;
            Assert::AreEqual((size_t)0, h.size());
            Assert::AreEqual((size_t)0, h.max_entries());
            h += "test";
            h += "test";
            Assert::AreEqual((size_t)2, h.size());
            h.clear();
            Assert::AreEqual((size_t)0, h.size());

            // Test repeating items
            h.set_allow_repeat_items(false);
            h += "test";
            h += "test2";
            h += "test";
            h += "test";
            Assert::AreEqual((size_t)3, h.size());
            Assert::AreEqual(std::string("test"),  h[0]);
            Assert::AreEqual(std::string("test2"), h[1]);
            Assert::AreEqual(std::string("test"),  h[2]);
            h.clear();
            h.set_allow_repeat_items(true);

            // Test deletion of old items when limit is reached
            h.set_max_entries(3);
            h.add("test1");
            h += "test4";
            h += "test3";
            Assert::AreEqual((size_t)3, h.size());
            Assert::AreEqual(std::string("test3"), h[0]);
            Assert::AreEqual(std::string("test1"), h[2]);
            h += "test2";
            Assert::AreEqual((size_t)3, h.size());
            Assert::AreEqual(std::string("test2"), h[0]);
            Assert::AreEqual(std::string("test4"), h[2]);
            h.set_max_entries(0);
            h += "test1";
            Assert::AreEqual((size_t)4, h.size());
            Assert::AreEqual(std::string("test1"), h[0]);
            Assert::AreEqual(std::string("test2"), h[1]);
            Assert::AreEqual(std::string("test3"), h[2]);
            Assert::AreEqual(std::string("test4"), h[3]);
        }
        
        TEST_METHOD(console_message)
        {
            using namespace sp::gui::console;
            message<char> msg;
            Assert::AreEqual((size_t)1, msg.line_count());
            Assert::AreEqual(std::string(), msg.str_raw());
            Assert::AreEqual(std::string("\n"), msg.str());

            msg = message<char>("test");
            Assert::AreEqual((size_t)1, msg.line_count());
            Assert::AreEqual(std::string("test"), msg.str_raw());

            msg = std::string("test\ntest");
            Assert::AreEqual((size_t)2, msg.line_count());
            Assert::AreEqual(std::string("test\ntest"), msg.str_raw());

            msg = "\n";
            Assert::AreEqual((size_t)2, msg.line_count());
            Assert::AreEqual(std::string("\n"), msg.str_raw());

            // Test overflow_mode::hide
            msg = message<char>("test\n2test\nt", 4);
            Assert::AreEqual((size_t)3, msg.line_count());
            Assert::AreEqual(std::string("test"), msg[0]);
            Assert::AreEqual(std::string("2tes"), msg[1]);
            Assert::AreEqual(std::string("t"), msg[2]);
            Assert::AreEqual(std::string("test\n2tes\nt\n"), msg.str());
            size_t lines = 0;
            Assert::AreEqual(std::string("test\n2tes\n"), msg.str(2, &lines));
            Assert::AreEqual((size_t)2, lines);

            // Test overflow_mode::wrap
            msg = message<char>("test\n2testing2\nt", 4, overflow_mode::wrap);
            Assert::AreEqual((size_t)5, msg.line_count());
            Assert::AreEqual(std::string("test"), msg[0]);
            Assert::AreEqual(std::string("2tes"), msg[1]);
            Assert::AreEqual(std::string("ting"), msg[2]);
            Assert::AreEqual(std::string("2"), msg[3]);
            Assert::AreEqual(std::string("t"), msg[4]);

            msg = message<char>("test", 1, overflow_mode::wrap);
            Assert::AreEqual((size_t)4, msg.line_count());
            Assert::AreEqual(std::string("t"), msg[0]);
            Assert::AreEqual(std::string("e"), msg[1]);
            Assert::AreEqual(std::string("s"), msg[2]);
            Assert::AreEqual(std::string("t"), msg[3]);
            Assert::AreEqual(std::string("t\ne\ns\nt\n"), msg.str());
        }

        TEST_METHOD(console_output)
        {
            using namespace sp::gui::console;
            output<> out(1, 5, overflow_mode::wrap);
            out.print("test");
            Assert::AreEqual(std::string("t\ne\ns\nt\n"), out.str());
            out.set_height(2);
            Assert::AreEqual(std::string("t\ne\n"), out.str());
            out.set_height(5);
            Assert::AreEqual(std::string("t\ne\ns\nt\n"), out.str());
            out.set_width(8);
            Assert::AreEqual(std::string("test\n"), out.str());
            out.print();
            Assert::AreEqual(std::string("test\n\n"), out.str());
            out.print("testing123");
            Assert::AreEqual(std::string("test\n\ntesting1\n23\n"), out.str());
            out.cls();
            Assert::AreEqual(std::string("\n\n\n\n\n"), out.str());
            out.print("1");
            out.print("2");
            out.print("3");
            out.print("4");
            out.print("5");
            Assert::AreEqual(std::string("1\n2\n3\n4\n5\n"), out.str());
        }

    };
}
