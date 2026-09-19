#include "low_pass_filter.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned checks = 0;
static unsigned failures = 0;
static unsigned cases = 0;
static unsigned failed_cases = 0;

static void check_near(double actual, double expected, double tolerance,
                       const char *expression, int line)
{
    ++checks;
    if (!isfinite(actual) || !(fabs(actual - expected) <= tolerance)) {
        ++failures;
        printf("  FAIL line %d: %s\n    expected %.17g, got %.17g (tolerance %.3g)\n",
               line, expression, expected, actual, tolerance);
    }
}

static void check_true(int condition, const char *expression, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("  FAIL line %d: %s\n", line, expression);
    }
}

#define NEAR(actual, expected, tolerance) \
    check_near((actual), (expected), (tolerance), #actual, __LINE__)
#define EQUAL(actual, expected) NEAR(actual, expected, 0.0f)
#define CHECK(expression) check_true((expression), #expression, __LINE__)

static void first_call(void)
{
    EQUAL(low_pass_filter(7.5f, 0.25f, 0), 7.5f);
    EQUAL(low_pass_filter(-0.5f, 0.25f, 0), 5.5f);
}

static void resets(void)
{
    EQUAL(low_pass_filter(8.0f, 0.1f, 1), 8.0f);
    EQUAL(low_pass_filter(-4.0f, 0.25f, 1), -4.0f);
    EQUAL(low_pass_filter(4.0f, 0.25f, 0), -2.0f);
    EQUAL(low_pass_filter(12.0f, 0.5f, 1), 12.0f);
    EQUAL(low_pass_filter(0.0f, 0.5f, 0), 6.0f);
}

static void changing_alpha(void)
{
    low_pass_filter(0.0f, 0.1f, 1);
    EQUAL(low_pass_filter(8.0f, 0.25f, 0), 2.0f);
    EQUAL(low_pass_filter(8.0f, 0.5f, 0), 5.0f);
    EQUAL(low_pass_filter(-3.0f, 1.0f, 0), -3.0f);
    EQUAL(low_pass_filter(5.0f, 0.25f, 0), -1.0f);
}

static void init_flag(void)
{
    low_pass_filter(0.0f, 0.5f, 1);
    EQUAL(low_pass_filter(8.0f, 0.5f, 2), 4.0f);
    EQUAL(low_pass_filter(0.0f, 0.5f, -1), 2.0f);
}

static void pass_through(void)
{
    const fp32 samples[] = {2.0f, -8.0f, 0.0f, 12.0f};
    low_pass_filter(0.0f, 0.5f, 1);
    for (unsigned i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i)
        EQUAL(low_pass_filter(samples[i], 1.0f, 0), samples[i]);
    EQUAL(low_pass_filter(0.0f, 0.5f, 0), 6.0f);
}

static void invalid_alpha(void)
{
    const fp32 alphas[] = {0.0f, -0.0f, -0.1f, -100.0f,
                            nextafterf(1.0f, 2.0f), 2.0f, INFINITY, -INFINITY};
    for (unsigned i = 0; i < sizeof(alphas) / sizeof(alphas[0]); ++i) {
        const unsigned before = failures;
        EQUAL(low_pass_filter(4.0f, alphas[i], 1), 4.0f);
        EQUAL(low_pass_filter(8.0f, 0.5f, 0), 6.0f);
        EQUAL(low_pass_filter(-2.0f, alphas[i], 0), -2.0f);
        EQUAL(low_pass_filter(6.0f, 0.25f, 0), 0.0f);
        if (failures != before)
            printf("    alpha under test: %.17g\n", alphas[i]);
    }
}

static void small_alpha(void)
{
    low_pass_filter(0.0f, 1e-12f, 1);
    NEAR(low_pass_filter(1.0f, 1e-12f, 0), 1e-12, 1e-18);
}

static void constant_input(void)
{
    const fp32 samples[] = {-10.0f, 0.0f, 3.5f};
    for (unsigned i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
        low_pass_filter(samples[i], 0.1f, 1);
        for (int n = 0; n < 100; ++n)
            NEAR(low_pass_filter(samples[i], 0.1f, 0), samples[i], 2e-5);
    }
}

static void step_response(void)
{
    const fp32 alphas[] = {0.01f, 0.1f, 0.5f, 1.0f};
    for (unsigned i = 0; i < sizeof(alphas) / sizeof(alphas[0]); ++i) {
        for (int sign = -1; sign <= 1; sign += 2) {
            const fp32 target = sign * 5.0f;
            const fp32 alpha = alphas[i];
            fp32 previous = 0.0f;
            low_pass_filter(0.0f, alpha, 1);
            for (int n = 1; n <= 500; ++n) {
                const fp32 y = low_pass_filter(target, alpha, 0);
                NEAR(y, target * (1.0 - pow(1.0 - (double)alpha, n)), 5e-5);
                CHECK(fabs(target - y) <= fabs(target - previous) + 2e-6);
                CHECK(fabs(y) <= fabs(target) + 2e-6);
                previous = y;
            }
        }
    }
}

static void impulse_response(void)
{
    low_pass_filter(0.0f, 0.25f, 1);
    EQUAL(low_pass_filter(4.0f, 0.25f, 0), 1.0f);
    for (int n = 1; n <= 100; ++n)
        NEAR(low_pass_filter(0.0f, 0.25f, 0), pow(0.75f, n), 2e-7);
}

static void mixed_signal(void)
{
    long double expected = -2.0L;
    low_pass_filter(-2.0f, 0.1f, 1);
    for (int n = 0; n < 1000; ++n) {
        const fp32 x = (fp32)((n * 37) % 101 - 50);
        const fp32 alpha = (n % 9 + 1) / 10.0f;
        expected = (1.0L - alpha) * expected + alpha * x;
        NEAR(low_pass_filter(x, alpha, 0), (double)expected, 2e-5);
    }
}

static void run_case(const char *name, void (*test)(void))
{
    const unsigned before = failures;
    ++cases;
    test();
    if (failures != before)
        ++failed_cases;
    printf("[%s] %s\n", failures == before ? "PASS" : "FAIL", name);
}

static void print_step_response(void)
{
    const fp32 alpha = 0.10f;
    puts("\nStep response: alpha = 0.10, input changes from 0 to 5 at sample 10");
    for (int n = 0; n < 64; ++n) {
        const fp32 x = n < 10 ? 0.0f : 5.0f;

        const fp32 y = low_pass_filter(x, alpha, n == 0);
        printf("%3d  in=%7.3f  out=%.8f\n", n, (double)x, (double)y);
    }
}

int main(void)
{
    puts("Low-pass filter correctness testbench");
    run_case("First call without init", first_call);
    run_case("Initialization and repeated reset", resets);
    run_case("Latest alpha on each update", changing_alpha);
    run_case("Only init == 1 reseeds", init_flag);
    run_case("Pass-through maintains state", pass_through);
    run_case("Invalid alpha clamping (8 values)", invalid_alpha);
    run_case("Small positive alpha", small_alpha);
    run_case("Constant input", constant_input);
    run_case("Rising and falling step responses", step_response);
    run_case("Impulse decay", impulse_response);
    run_case("Mixed signal reference", mixed_signal);
    printf("\nResult: %u/%u cases passed; %u checks, %u failures.\n",
           cases - failed_cases, cases, checks, failures);
    if (failures == 0) {
        print_step_response();
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

