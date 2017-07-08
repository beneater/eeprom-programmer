using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;

namespace multiplexed_display.tests
{
    [TestClass]
    public class DisplayTests
    {
        private readonly char[] _digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        private const char Blank = ' ';
        private const char MinusSign = '-';
        private const char HexSigh = 'h';

        private static string Display(int i, IReadOnlyList<char> data)
        {
            return $"{data[0x300 + i]}{data[0x200 + i]}{data[0x100 + i]}{data[0x000 + i]}";
        }

        [TestMethod]
        public void UnsingedTest()
        {
            var data = new char[0x400];

            for (var i = 0; i < 0x100; i++)
            {
                var d2 = i / 100 % 10; // 2nd 7segment display (hundreds)
                var d3 = i / 10 % 10; // 3rd 7segment display (tenths)
                data[i + 0x300] = Blank; // clear first 7segment display
                data[i + 0x200] = d2 == 0 ? Blank : _digits[d2]; // write the digit or clear if 0
                data[i + 0x100] = d2 == 0 && d3 == 0 ? Blank : _digits[d3]; // write the digit or clear if d3 and d2 are 0
                data[i + 0x000] = _digits[i % 10]; // write the digit even 0
            }

            Assert.AreEqual("   0", Display(0, data));
            Assert.AreEqual("   7", Display(7, data));
            Assert.AreEqual("  16", Display(16, data));
            Assert.AreEqual(" 100", Display(100, data));
            Assert.AreEqual(" 255", Display(255, data));
        }

        [TestMethod]
        public void HexTest()
        {
            var data = new char[0x400];

            for (var i = 0; i < 0x100; i++)
            {
                var d2 = i >> 4;
                var d3 = i & 0x0F;
                data[i + 0x300] = Blank;
                data[i + 0x200] = _digits[d2];
                data[i + 0x100] = _digits[d3];
                data[i + 0x000] = HexSigh;
            }

            Assert.AreEqual(" 00h", Display(0x00, data));
            Assert.AreEqual(" 07h", Display(0x07, data));
            Assert.AreEqual(" 1ch", Display(0x1C, data));
            Assert.AreEqual(" 64h", Display(0x64, data));
            Assert.AreEqual(" ffh", Display(0xFF, data));
        }

        [TestMethod]
        public void TwosCompletentTest()
        {
            var data = new char[0x400];

            for (var i = 0; i < 0x100; i++)
            {
                if (i < 0x80)
                {
                    var d2 = i / 100 % 10; // 2nd 7segment display (hundreds)
                    var d3 = i / 10 % 10; // 3rd 7segment display (tenths)
                    data[i + 0x300] = Blank; // clear first 7segment display
                    data[i + 0x200] = d2 == 0 ? Blank : _digits[d2]; // write the digit or clear if 0
                    data[i + 0x100] = d2 == 0 && d3 == 0 ? Blank : _digits[d3]; // write the digit or clear if d3 and d2 are 0
                    data[i + 0x000] = _digits[i % 10]; // write the digit even 0
                }
                else
                {
                    // Example
                    // d1 d2 d3 d4
                    //  -  1  2  3
                    //     -  4  2
                    //        -  7

                    var value = i - 0x100; //real value as two's complement
                    var d2 = Math.Abs(value) / 100 % 10; //hundreds
                    var d3 = Math.Abs(value) / 10 % 10; //tenths
                    var d4 = Math.Abs(value) % 10; //ones

                    //first 7segment display
                    data[i + 0x300] = d2 == 0 ? Blank : MinusSign; //write a minus sign to the first 7segment display if the second 7segment display isn't clear

                    //second 7segment display
                    if (d2 == 0 && d3 == 0)
                    { // if the minus sign is further right
                        data[i + 0x200] = Blank; // clear second 7segment display
                    }
                    else
                    {
                        data[i + 0x200] = d2 == 0 ? MinusSign : _digits[d2];
                    }

                    // third 7segment display
                    data[i + 0x100] = d2 == 0 && d3 == 0 ? MinusSign : _digits[d3]; // write a minus sign if both the second and the third 7segment display are clear

                    // last 7segment display
                    data[i + 0x000] = _digits[d4]; // always write the last digit (ones)
                }
            }

            Assert.AreEqual("   0", Display(0, data));
            Assert.AreEqual("  16", Display(16, data));
            Assert.AreEqual(" 127", Display(127, data));
            Assert.AreEqual("-128", Display(128, data));
            Assert.AreEqual(" -10", Display(246, data));
            Assert.AreEqual("  -1", Display(255, data));
        }

        private static uint BinaryToGray(uint num)
        {
            return num ^ (num >> 1);
        }

        private static uint GrayToBinary32(uint num)
        {
            num = num ^ (num >> 16);
            num = num ^ (num >> 8);
            num = num ^ (num >> 4);
            num = num ^ (num >> 2);
            num = num ^ (num >> 1);
            return num;
        }

        [TestMethod]
        public void GrayTest()
        {
            var data = new char[0x400];

            for (var gray = 0x000; gray < 0x100; gray++)
            {
                var binary = GrayToBinary32((uint)gray);

                var d2 = binary / 100 % 10; // 2nd 7segment display (hundreds)
                var d3 = binary / 10 % 10; // 3rd 7segment display (tenths)
                data[gray + 0x300] = Blank; // clear first 7segment display
                data[gray + 0x200] = d2 == 0 ? Blank : _digits[d2]; // write the digit or clear if 0
                data[gray + 0x100] = d2 == 0 && d3 == 0 ? Blank : _digits[d3]; // write the digit or clear if d3 and d2 are 0
                data[gray + 0x000] = _digits[binary % 10]; // write the digit even 0
            }

            for (var binary = 0x000; binary < 0x100; binary++)
            {
                var gray = BinaryToGray((uint)binary);
                Assert.AreEqual($"{binary,4:##0}", Display((int)gray, data));
            }
        }

        [TestMethod]
        public void BcdTest()
        {
            var data = new char[0x400];

            for (var i = 0; i < 0x100; i++)
            {
                var d3 = i >> 4;
                var d4 = i & 0x0F;

                if (d3 > 9 || d4 > 9)
                {
                    data[i + 0x300] = MinusSign;
                    data[i + 0x200] = MinusSign;
                    data[i + 0x100] = MinusSign;
                    data[i + 0x000] = MinusSign;
                    continue;
                }

                data[i + 0x300] = Blank;
                data[i + 0x200] = Blank;
                data[i + 0x100] = d3 == 0 ? Blank : _digits[d3];
                data[i + 0x000] = _digits[d4];
            }

            Assert.AreEqual("   0", Display(0x00, data));
            Assert.AreEqual("   7", Display(0x07, data));
            Assert.AreEqual("  26", Display(0x26, data));
            Assert.AreEqual("  99", Display(0x99, data));
            Assert.AreEqual("----", Display(0x0A, data));
            Assert.AreEqual("----", Display(0xA0, data));
        }
    }
}
