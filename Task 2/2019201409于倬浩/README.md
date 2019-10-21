# My expression evaluator with self-implemented FP support.

The input function `readFP()` uses pre-calculated 10^k FP to process input data, without losing too much precision.

The output function `printFP()` uses a simple BigInt type to handle large numbers.

To modify Input and Output precision, just change constant `PREC_FIX` to another value. By default, precision is up to 2^(-30).
