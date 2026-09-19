/* Low-pass filter challenge implementation. */
#include "low_pass_filter.h"


/* =======================================================================
 *  TASK: Implement a single-function EMA smoother with internal state.
 *
 *  Function signature:
 *      fp32 low_pass_filter(fp32 x, fp32 alpha, int init);
 *
 *  Usage (call once per control-loop tick):
 *      y = low_pass_filter(x, alpha, init);
 *
 *  Parameters:
 *      x      : current input sample
 *      alpha  : smoothing factor in (0, 1]; smaller -> smoother/slower
 *                (alpha = 1.0f -> pass-through / no smoothing)
 *      init   : set to 1 to (re)initialize; seeds y = x and stores alpha
 *               (use 0 for normal updates)
 *
 *  Requirements:
 *    - Keep static internal state: initialized flag, last output y, last alpha.
 *    - Clamp alpha into (0, 1]; if alpha <= 0 or alpha > 1, set alpha = 1.0f.
 *    - On first call OR when init == 1:
 *        * store alpha
 *        * y <- x   (seed output)
 *        * return y
 *    - Otherwise (normal update):
 *        * y <- (1 - alpha) * y + alpha * x
 *        * return y
 * ======================================================================= */

fp32 low_pass_filter(fp32 x, fp32 alpha, int init)
{
    /* =============================
     *  Put your implementation here
     * ============================= */

     return 0.0f; /* Placeholder: implement the filter to pass the tests. */
}


