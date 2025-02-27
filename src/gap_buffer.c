#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAP_BUFFER_SIZE 64

#define canbe_null

typedef struct
{
    char *buffer;
    size_t size;
    int start;
    int end;
} GapBuffer;

// int gap_buffer_gap_len(const GapBuffer* gb);
// int gap_buffer_content_len(const GapBuffer* gb);
// char gap_buffer_at(const GapBuffer* gb,const size_t index);

static inline int gap_buffer_gap_len(const GapBuffer* gb)
{
    return gb->end - gb->start + 1;
}

static inline int gap_buffer_content_len(const GapBuffer* gb)
{
    return gb->size - gap_buffer_gap_len(gb); 
}

//static inline
char gap_buffer_at(const GapBuffer* gb,const size_t index)
{
    if(index >= gap_buffer_content_len(gb)) return '\0';
    if(index < gb->start) return gb->buffer[index];  
    return gb->buffer[index + gap_buffer_gap_len(gb)]; 
}

GapBuffer *gap_buffer_init(canbe_null GapBuffer *gb , size_t gap_size)
{
    if (!gap_size)
        return NULL;

    void *buffer = calloc(gap_size,sizeof(*gb->buffer));

    if (!buffer)
        return NULL;

    if (!gb)
    {
        GapBuffer *gb_temp = malloc(sizeof(*gb_temp));

        if (!gb_temp)
        {
            free(buffer);
            return NULL;
        }

        gb_temp->buffer = buffer;
        gb_temp->size   = gap_size;
        gb_temp->start  = 0;
        gb_temp->end    = gap_size - 1;

        return gb_temp; 
    }

    gb->buffer = buffer;
    gb->size   = gap_size;
    gb->start  = 0;
    gb->end    = gap_size - 1;

    return gb;

}

void gap_buffer_move_right(GapBuffer *gb)
{
    if(gb->end + 1 <  gb->size)
    {
        gb->buffer[gb->start++] = gb->buffer[++gb->end];
    }
}

void gap_buffer_move_left(GapBuffer *gb)
{
    if(gb->start - 1 >= 0)
    {
        gb->buffer[gb->end--] = gb->buffer[--gb->start];
    }
}

void gap_buffer_seek(GapBuffer* gb, const size_t pos)
{
    if(pos > gap_buffer_content_len(gb)) return;
    size_t len = 0;  

    if(pos < gb->start)
    {
        len = gb->start - pos;
        const int gap_size = gap_buffer_gap_len(gb);
        // while(len--) gap_buffer_move_left(gb);
        memmove(&gb->buffer[pos + gap_size],&gb->buffer[pos],len);
        gb->start -= len;
        gb->end   -= len;
        return;
    }
    len = pos - gb->start;
    // while(len--) gap_buffer_move_right(gb);
    memmove(&gb->buffer[gb->start],&gb->buffer[gb->end + 1],len);
    gb->start += len;
    gb->end   += len;
}

#if 0
static int gap_buffer_print(GapBuffer* gb)
{
    int total_bytes = fwrite(gb->buffer,1,gb->start,stdout);

    if(gb->end+1 < gb->size)
        return total_bytes +  fwrite(&gb->buffer[gb->end + 1],1,gb->size - gb->end - 1,stdout);
    return total_bytes;
}

static void gap_buffer_visualize(GapBuffer* gb)
{
    for(int i = 0 ; i < gb->size;i++)
    {
        if(i >= gb->start && i <= gb->end)
        {
            printf("_");
        }
        else{
            printf("%c",gb->buffer[i]);
        }
    }
    printf("\n");
}

#endif

char gap_buffer_delete(GapBuffer* gb)
{
    if(gb->start == 0)
        return '\0';

    return gb->buffer[--gb->start];
}

void gap_buffer_insert(GapBuffer* gb, const char c)
{

    //gap is full
    if(gb->start ==  gb->end)
    {
        const int old_size = gb->size; 
        const int new_size = old_size * 2; 
        const int delta = old_size - gb->end - 1; 

        gb->buffer = realloc(gb->buffer,new_size);
        gb->size   = new_size;
        gb->end    = new_size - delta- 1;
        memcpy(&gb->buffer[gb->end+1],&gb->buffer[old_size - delta], delta); 
    }

    gb->buffer[gb->start++] = c;

}
