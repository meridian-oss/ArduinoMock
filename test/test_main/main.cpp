#include <unity.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// ---------------------------------------------------------------------------
// setUp / tearDown (required by Unity)
// ---------------------------------------------------------------------------
void setUp()    {}
void tearDown() {}

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
void test_constants() {
    TEST_ASSERT_EQUAL(1, HIGH);
    TEST_ASSERT_EQUAL(0, LOW);
    TEST_ASSERT_EQUAL(0, INPUT);
    TEST_ASSERT_EQUAL(1, OUTPUT);
    TEST_ASSERT_EQUAL(2, INPUT_PULLUP);
    TEST_ASSERT_EQUAL(13, LED_BUILTIN);
    TEST_ASSERT_EQUAL(0, LSBFIRST);
    TEST_ASSERT_EQUAL(1, MSBFIRST);
}

// ---------------------------------------------------------------------------
// Digital I/O
// ---------------------------------------------------------------------------
void test_digital_io() {
    // Default value is LOW (0)
    TEST_ASSERT_EQUAL(LOW, digitalRead(2));

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(2));

    digitalWrite(2, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(2));

    // Test helper: inject a value directly
    setDigitalValue(3, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(3));
}

// ---------------------------------------------------------------------------
// Analog I/O
// ---------------------------------------------------------------------------
void test_analog_io() {
    // Default is 0
    TEST_ASSERT_EQUAL(0, analogRead(0));

    analogWrite(5, 128);
    TEST_ASSERT_EQUAL(128, analogRead(5));

    setAnalogValue(6, 512);
    TEST_ASSERT_EQUAL(512, analogRead(6));
}

// ---------------------------------------------------------------------------
// Math macros
// ---------------------------------------------------------------------------
void test_math_macros() {
    TEST_ASSERT_EQUAL(3,  min(3, 7));
    TEST_ASSERT_EQUAL(7,  max(3, 7));
    TEST_ASSERT_EQUAL(5,  abs(-5));
    TEST_ASSERT_EQUAL(5,  abs(5));
    TEST_ASSERT_EQUAL(10, constrain(10, 0, 20));
    TEST_ASSERT_EQUAL(0,  constrain(-5, 0, 20));
    TEST_ASSERT_EQUAL(20, constrain(25, 0, 20));
    TEST_ASSERT_EQUAL(4,  sq(2));
    TEST_ASSERT_EQUAL(1UL, bit(0));
    TEST_ASSERT_EQUAL(8UL, bit(3));
}

// ---------------------------------------------------------------------------
// map()
// ---------------------------------------------------------------------------
void test_map() {
    TEST_ASSERT_EQUAL(0,   map(0,   0, 1023, 0, 255));
    TEST_ASSERT_EQUAL(255, map(1023, 0, 1023, 0, 255));
    TEST_ASSERT_EQUAL(127, map(511, 0, 1023, 0, 255));
    TEST_ASSERT_EQUAL(50,  map(5, 0, 10, 0, 100));
}

// ---------------------------------------------------------------------------
// Bit / byte macros
// ---------------------------------------------------------------------------
void test_bit_byte_macros() {
    TEST_ASSERT_EQUAL(0x34, lowByte(0x1234));
    TEST_ASSERT_EQUAL(0x12, highByte(0x1234));

    uint8_t val = 0;
    bitSet(val, 3);
    TEST_ASSERT_EQUAL(8, val);
    TEST_ASSERT_EQUAL(1, bitRead(val, 3));
    TEST_ASSERT_EQUAL(0, bitRead(val, 2));
    bitClear(val, 3);
    TEST_ASSERT_EQUAL(0, val);
    bitWrite(val, 5, 1);
    TEST_ASSERT_EQUAL(32, val);
    bitToggle(val, 5);
    TEST_ASSERT_EQUAL(0, val);
}

// ---------------------------------------------------------------------------
// Random numbers
// ---------------------------------------------------------------------------
void test_random() {
    randomSeed(42);
    for (int i = 0; i < 20; i++) {
        long r = random(100);
        TEST_ASSERT_TRUE(r >= 0 && r < 100);
    }
    for (int i = 0; i < 20; i++) {
        long r = random(10, 20);
        TEST_ASSERT_TRUE(r >= 10 && r < 20);
    }
    // Edge: random(0) should return 0
    TEST_ASSERT_EQUAL(0, random(0));
    // Edge: howsmall >= howbig returns howsmall
    TEST_ASSERT_EQUAL(5, random(5, 5));
}

// ---------------------------------------------------------------------------
// Time (just verify they return monotonically increasing values)
// ---------------------------------------------------------------------------
void test_time() {
    unsigned long t0 = millis();
    unsigned long u0 = micros();

    // millis / micros should be >= 0 (they're unsigned so always true,
    // but we can verify they advance after a small delay)
    delay(2);
    unsigned long t1 = millis();
    TEST_ASSERT_TRUE(t1 >= t0);

    unsigned long u1 = micros();
    TEST_ASSERT_TRUE(u1 >= u0);
}

// ---------------------------------------------------------------------------
// Character classification
// ---------------------------------------------------------------------------
void test_characters() {
    TEST_ASSERT_TRUE(isAlpha('a'));
    TEST_ASSERT_FALSE(isAlpha('1'));
    TEST_ASSERT_TRUE(isAlphaNumeric('a'));
    TEST_ASSERT_TRUE(isAlphaNumeric('3'));
    TEST_ASSERT_FALSE(isAlphaNumeric('!'));
    TEST_ASSERT_TRUE(isAscii(0));
    TEST_ASSERT_TRUE(isAscii(127));
    TEST_ASSERT_FALSE(isAscii(128));
    TEST_ASSERT_TRUE(isDigit('5'));
    TEST_ASSERT_FALSE(isDigit('a'));
    TEST_ASSERT_TRUE(isHexadecimalDigit('F'));
    TEST_ASSERT_TRUE(isHexadecimalDigit('9'));
    TEST_ASSERT_FALSE(isHexadecimalDigit('G'));
    TEST_ASSERT_TRUE(isLowerCase('a'));
    TEST_ASSERT_FALSE(isLowerCase('A'));
    TEST_ASSERT_TRUE(isUpperCase('A'));
    TEST_ASSERT_FALSE(isUpperCase('a'));
    TEST_ASSERT_TRUE(isWhitespace(' '));
    TEST_ASSERT_FALSE(isWhitespace('a'));
    TEST_ASSERT_TRUE(isPunct('!'));
    TEST_ASSERT_TRUE(isControl('\t'));
    TEST_ASSERT_TRUE(isPrintable('A'));
    TEST_ASSERT_FALSE(isPrintable('\x01'));
    TEST_ASSERT_TRUE(isGraph('A'));
    TEST_ASSERT_FALSE(isGraph(' '));
}

// ---------------------------------------------------------------------------
// String – constructors
// ---------------------------------------------------------------------------
void test_string_constructors() {
    String s1("hello");
    TEST_ASSERT_EQUAL_STRING("hello", s1.c_str());

    String s2(42);
    TEST_ASSERT_EQUAL_STRING("42", s2.c_str());

    String s3(3.14f, 2);
    // Verify it starts with "3.14"
    TEST_ASSERT_EQUAL(0, strncmp("3.14", s3.c_str(), 4));

    String s4('X');
    TEST_ASSERT_EQUAL_STRING("X", s4.c_str());

    String s5(255u, 16);   // unsigned hex
    // hex representation of 255 is "ff"
    TEST_ASSERT_EQUAL_STRING("ff", s5.c_str());

    String s6;
    TEST_ASSERT_TRUE(s6.isEmpty());
}

// ---------------------------------------------------------------------------
// String – concatenation
// ---------------------------------------------------------------------------
void test_string_concat() {
    String s = "Hello";
    s += ", ";
    s += "World";
    TEST_ASSERT_EQUAL_STRING("Hello, World", s.c_str());

    String s2 = String("A") + String("B") + String("C");
    TEST_ASSERT_EQUAL_STRING("ABC", s2.c_str());

    String s3;
    s3.concat("foo");
    s3.concat(42);
    TEST_ASSERT_EQUAL_STRING("foo42", s3.c_str());
}

// ---------------------------------------------------------------------------
// String – search
// ---------------------------------------------------------------------------
void test_string_search() {
    String s("Hello, World");
    TEST_ASSERT_EQUAL(0, s.indexOf('H'));
    TEST_ASSERT_EQUAL(7, s.indexOf('W'));
    TEST_ASSERT_EQUAL(-1, s.indexOf('z'));
    TEST_ASSERT_EQUAL(7, s.indexOf(String("World")));
    TEST_ASSERT_EQUAL(11, s.lastIndexOf('d'));
    TEST_ASSERT_EQUAL(8, s.lastIndexOf('o'));  // last 'o' is in "World" at index 8
    TEST_ASSERT_TRUE(s.startsWith(String("Hello")));
    TEST_ASSERT_FALSE(s.startsWith(String("World")));
    TEST_ASSERT_TRUE(s.endsWith(String("World")));
    TEST_ASSERT_FALSE(s.endsWith(String("Hello")));
}

// ---------------------------------------------------------------------------
// String – transformation
// ---------------------------------------------------------------------------
void test_string_transform() {
    String s("Hello World");
    s.toLowerCase();
    TEST_ASSERT_EQUAL_STRING("hello world", s.c_str());
    s.toUpperCase();
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD", s.c_str());

    String t("  trim me  ");
    t.trim();
    TEST_ASSERT_EQUAL_STRING("trim me", t.c_str());

    String r("aabbcc");
    r.replace('b', 'x');
    TEST_ASSERT_EQUAL_STRING("aaxxcc", r.c_str());

    String r2("foo bar foo");
    r2.replace(String("foo"), String("baz"));
    TEST_ASSERT_EQUAL_STRING("baz bar baz", r2.c_str());
}

// ---------------------------------------------------------------------------
// String – substring / remove
// ---------------------------------------------------------------------------
void test_string_substring_remove() {
    String s("Hello, World");
    // Store substring results to avoid dangling-pointer UB in the test macro.
    String sub1 = s.substring(7);
    String sub2 = s.substring(0, 5);
    TEST_ASSERT_EQUAL_STRING("World", sub1.c_str());
    TEST_ASSERT_EQUAL_STRING("Hello", sub2.c_str());

    String r("Hello, World");
    r.remove(5);
    TEST_ASSERT_EQUAL_STRING("Hello", r.c_str());

    String r2("Hello, World");
    r2.remove(5, 7);
    TEST_ASSERT_EQUAL_STRING("Hello", r2.c_str());
}

// ---------------------------------------------------------------------------
// String – conversion
// ---------------------------------------------------------------------------
void test_string_conversion() {
    String s("42");
    TEST_ASSERT_EQUAL(42, s.toInt());

    String f("3.14");
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.14f, f.toFloat());

    String d("2.718");
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.718, static_cast<float>(d.toDouble()));
}

// ---------------------------------------------------------------------------
// String – comparison
// ---------------------------------------------------------------------------
void test_string_comparison() {
    String a("abc");
    String b("abc");
    String c("xyz");

    TEST_ASSERT_TRUE(a == b);
    TEST_ASSERT_FALSE(a == c);
    TEST_ASSERT_TRUE(a != c);
    TEST_ASSERT_TRUE(a <  c);
    TEST_ASSERT_TRUE(c >  a);

    String lower("hello");
    String upper("HELLO");
    TEST_ASSERT_TRUE(lower.equalsIgnoreCase(upper));
    TEST_ASSERT_FALSE(lower.equals(upper));
}

// ---------------------------------------------------------------------------
// PROGMEM macros (just confirm they compile and return the right values)
// ---------------------------------------------------------------------------
void test_progmem() {
    const char msg[] PROGMEM = "test";
    TEST_ASSERT_EQUAL_STRING("test", msg);

    const char *p = PSTR("hello");
    TEST_ASSERT_EQUAL_STRING("hello", p);

    const char *f = F("world");
    TEST_ASSERT_EQUAL_STRING("world", f);

    uint8_t  b  = 0xAB;
    uint16_t w  = 0x1234;
    uint32_t dw = 0xDEADBEEF;
    float    fl = 1.5f;

    TEST_ASSERT_EQUAL(0xAB,       pgm_read_byte(&b));
    TEST_ASSERT_EQUAL(0x1234,     pgm_read_word(&w));
    TEST_ASSERT_EQUAL(0xDEADBEEF, pgm_read_dword(&dw));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, pgm_read_float(&fl));
}

// ---------------------------------------------------------------------------
// SPI (compile + runtime smoke test)
// ---------------------------------------------------------------------------
void test_spi() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    uint8_t rx = SPI.transfer(0x55);
    TEST_ASSERT_EQUAL(0xFF, rx);
    uint16_t rx16 = SPI.transfer16(0x1234);
    TEST_ASSERT_EQUAL(0xFFFF, rx16);
    SPI.endTransaction();
    SPI.end();
}

// ---------------------------------------------------------------------------
// Wire (compile + runtime smoke test)
// ---------------------------------------------------------------------------
void test_wire() {
    Wire.begin();
    Wire.setClock(400000);
    Wire.beginTransmission(0x3C);
    Wire.write(static_cast<uint8_t>(0x00));
    uint8_t err = Wire.endTransmission();
    TEST_ASSERT_EQUAL(I2C_ERROR_NONE, err);

    uint8_t cnt = Wire.requestFrom(0x3C, 2);
    TEST_ASSERT_EQUAL(0, cnt);
    TEST_ASSERT_EQUAL(0, Wire.available());
    Wire.end();
}

// ---------------------------------------------------------------------------
// Serial (compile + runtime smoke test)
// ---------------------------------------------------------------------------
void test_serial() {
    Serial.begin(9600);
    TEST_ASSERT_TRUE(static_cast<bool>(Serial));

    // These write to stdout; we just verify they don't crash.
    Serial.print("Hello ");
    Serial.println("World");
    Serial.print(42);
    Serial.println(3.14, 2);
    Serial.print(255, HEX);
    Serial.println();

    Serial.end();
    TEST_ASSERT_FALSE(static_cast<bool>(Serial));
}

// ---------------------------------------------------------------------------
// Interrupt stubs (compile only – no hardware)
// ---------------------------------------------------------------------------
static void myISR() {}

void test_interrupts() {
    attachInterrupt(digitalPinToInterrupt(2), myISR, RISING);
    detachInterrupt(digitalPinToInterrupt(2));
    interrupts();
    noInterrupts();
    // Just verifies these compile and don't crash
    TEST_PASS();
}

// ---------------------------------------------------------------------------
// radians / degrees helpers
// ---------------------------------------------------------------------------
void test_trig_helpers() {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, static_cast<float>(PI), radians(180.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, degrees(static_cast<float>(PI)));
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main(int /*argc*/, char ** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(test_constants);
    RUN_TEST(test_digital_io);
    RUN_TEST(test_analog_io);
    RUN_TEST(test_math_macros);
    RUN_TEST(test_map);
    RUN_TEST(test_bit_byte_macros);
    RUN_TEST(test_random);
    RUN_TEST(test_time);
    RUN_TEST(test_characters);
    RUN_TEST(test_string_constructors);
    RUN_TEST(test_string_concat);
    RUN_TEST(test_string_search);
    RUN_TEST(test_string_transform);
    RUN_TEST(test_string_substring_remove);
    RUN_TEST(test_string_conversion);
    RUN_TEST(test_string_comparison);
    RUN_TEST(test_progmem);
    RUN_TEST(test_spi);
    RUN_TEST(test_wire);
    RUN_TEST(test_serial);
    RUN_TEST(test_interrupts);
    RUN_TEST(test_trig_helpers);

    return UNITY_END();
}
