#define CBUF_IMPLEMENTATION

#ifndef CBUF_H_
#define CBUF_H_
typedef struct circular_buf_t
{
    unsigned char *    buffer;
    unsigned long long head;
    unsigned long long tail;
    unsigned long long size;
} circular_buf_t;

void               circular_buf_init(circular_buf_t *cbuf, unsigned char *buffer, unsigned long long size);
void               circular_buf_reset(circular_buf_t *cbuf);
void               circular_buf_put(circular_buf_t *cbuf, unsigned char data);
void               circular_buf_get(circular_buf_t *cbuf, unsigned char *data);
int                circular_buf_full(circular_buf_t *cbuf);
int                circular_buf_empty(circular_buf_t *cbuf);
unsigned long long circular_buf_size(circular_buf_t *cbuf);
#endif // CBUF_H_

#ifdef CBUF_IMPLEMENTATION
void circular_buf_init(circular_buf_t *cbuf, unsigned char *buffer, unsigned long long size)
{
    cbuf->buffer = buffer;
    cbuf->size = size;
    circular_buf_reset(cbuf);
}

void circular_buf_reset(circular_buf_t *cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
}

void circular_buf_put(circular_buf_t *cbuf, unsigned char data)
{
    cbuf->buffer[cbuf->head] = data;
    cbuf->head = (cbuf->head + 1) % cbuf->size;
}

void circular_buf_get(circular_buf_t *cbuf, unsigned char *data)
{
    *data = cbuf->buffer[cbuf->tail];
    cbuf->tail = (cbuf->tail + 1) % cbuf->size;
}

unsigned long long circular_buf_size(circular_buf_t *cbuf)
{
    unsigned long long size = cbuf->size;
    if (!circular_buf_full(cbuf))
    {
        if (cbuf->head >= cbuf->tail)
        {
            size = (cbuf->head - cbuf->tail);
        }
        else
        {
            size = (cbuf->size + cbuf->head - cbuf->tail);
        }
    }
    return size;
}

int circular_buf_full(circular_buf_t *cbuf)
{
    unsigned long long head = cbuf->head;
    if (head == cbuf->size)
    {
        head = 0;
    }
    return head == cbuf->tail;
}

int circular_buf_empty(circular_buf_t *cbuf)
{
    return (cbuf->head == cbuf->tail);
}
#endif // CBUF_IMPLEMENTATION

int printf(const char *_Format, ...);

int main()
{
    circular_buf_t cbuf;
    unsigned char  buffer[5];
    circular_buf_init(&cbuf, buffer, sizeof(buffer));
    for (unsigned long long i = 0; i <= sizeof(buffer); ++i)
    {
        circular_buf_put(&cbuf, 49 + i);
        printf("put %c ", (unsigned char)(49 + i));
        printf("size %llu ", circular_buf_size(&cbuf));
        printf("full %d\n", circular_buf_full(&cbuf));
    }
    unsigned char out = 0;
    circular_buf_get(&cbuf, &out);
    printf("get %c\n", out);
    return 0;
}