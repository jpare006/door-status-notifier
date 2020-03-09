/** @file CircularBuffer.c
 * 
 * @brief 
 * Circular buffer library for embedded systems. Supports uint8_t elements.
 * Functions can be easily modified to support the required data type. 
 * 
 */

#include "CircularBuffer.h"

struct circular_buf_t
{
    // Reads from the circular buffer happen at the index kept in "head" of the buffer.
    // Writes happen at the tail of the buffer.
    //
    TYPE * p_buffer;
    size_t    head;
    size_t    tail;
    size_t    max; // This variable represents the maximum size of the buffer.
    BOOL      b_is_buffer_full;
};

enum {TRUE = 1, FALSE = 0};

/*!
 * @brief Advance the circular buffer tail index.
 * @param[in] cbuf Handle for circular buffer.
 *
 * @par 
 * This function is always called when a write to the circular buffer has been
 * made and the value has already been written to the underlying buffer.
 */
static void advance_tail (cbuf_handle_t cbuf)
{
    if (cbuf->b_is_buffer_full)
    {
        //The head index must also be advanced during a data overwrite.
        //
        cbuf->head = (cbuf->head + 1) % cbuf->max;
    }

    cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    cbuf->b_is_buffer_full = (cbuf->head == cbuf->tail);
}

/*!
 * @brief Advance the circular buffer head index.
 * @param[in] cbuf Handle for circular buffer.
 *
 * @par 
 * This function is always called when a circular buffer read is made. It is
 * called after the data has already been read.
 */
static void advance_head (cbuf_handle_t cbuf)
{
    cbuf->head = (cbuf->head + 1) % cbuf->max;
    cbuf->b_is_buffer_full = FALSE;
}

/*!
 * @brief 
 * Initialize the circular buffer structure with user declared buffer and size.
 * 
 * @param[in] p_buffer A pointer to the user declared buffer.
 * @param[in] size     The size of the buffer.
 * @return The handle type used to access the circular buffer internals.
 */
cbuf_handle_t circular_buf_init (TYPE * p_buffer, size_t size)
{
    cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
    cbuf->p_buffer = p_buffer;
    cbuf->max = size;
    circular_buf_reset(cbuf);

    return cbuf;
}

/*!
 * @brief Check if circular buffer is empty or not.
 * @param[in] cbuf Handle for circular buffer.
 * @return Boolean value answering if circular buffer is empty or not.
 */
BOOL circular_buf_empty (cbuf_handle_t cbuf)
{
    if((cbuf->head == cbuf->tail) && !cbuf->b_is_buffer_full)
    {
        return TRUE;
    }

    return FALSE;
}

/*!
 * @brief Check if circular buffer is full or not.
 * @param[in] cbuf Handle for circular buffer.
 * @return Boolean value answering if circular buffer is full or not.
 */
BOOL circular_buf_full (cbuf_handle_t cbuf)
{
    // b_is_buffer_full is updated in other functions, therefore, this function 
    // simply returns whatever value is stored in the struct.
    return cbuf->b_is_buffer_full;
}

/*!
 * @brief Place a single data element into the underlying buffer.
 * @param[in] cbuf Handle for circular buffer.
 * @param[in] data Data element to be stored in circular buffer.
 */
void circular_buf_put (cbuf_handle_t cbuf, TYPE data)
{
    cbuf->p_buffer[cbuf->tail] = data;

    advance_tail(cbuf);
}

/*!
 * @brief Find the maximum number of elements the circular buffer can store.
 * @param[in] cbuf Handle for ciruclar buffer.
 * @return Maximum size of the circular buffer.
 */
size_t circular_buf_capacity (cbuf_handle_t cbuf)
{
    return cbuf->max;
}

/*!
 * @brief Reset the circular buffer to its initial state.
 * @param[in] cbuf Handle for circular buffer.
 */
void circular_buf_reset (cbuf_handle_t cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->b_is_buffer_full = FALSE;
}

/*!
 * @brief Read the next value in the circular buffer.
 * @param[in] cbuf    Handle for circular buffer.
 * @param[in] p_value Pointer to var where value that is read is to be stored.
 * @return Status indicating wether read was succesful (0) or not (-1).
 */
int circular_buf_get (cbuf_handle_t cbuf, TYPE * p_value)
{
    int status = -1;

    if(!circular_buf_empty(cbuf))
    {
        status = 0;

        *p_value = cbuf->p_buffer[cbuf->head];
        advance_head(cbuf);
    }

    return status;
}

/*!
 * @brief Calculate the current number of elements in the circular buffer.
 * @param[in] cbuf Handle for circular buffer.
 * @return Number of elements currently in circular buffer.
 */
size_t circular_buf_size (cbuf_handle_t cbuf)
{
    size_t value = 0;

    if(circular_buf_full(cbuf))
    {
        value = cbuf->max;
    }
    else if(cbuf->tail > cbuf->head)
    {
        value = cbuf->tail - cbuf->head;
    }
    else if (cbuf->tail < cbuf->head)
    {
        //Use buffer max value to calculate amount of elements
        value = (cbuf->max - cbuf->head) + cbuf->tail;
    }
    else
    {
        //At this point, the only option left is: (tail == head  == 0) && !full
        value = 0;
    }

    return value;
}

/*!
 * @brief Free memory that was allocated during initialization.
 * @param[in] cbuf Hanlde for circular buffer.
 * 
 * @par 
 * It is the users responsibility to call this function at the end of the
 * program once for every time circular_buf_init was called.
 */
int circular_buf_free (cbuf_handle_t cbuf)
{
    free(cbuf);

    return 0;
}

/*** end of file ***/