#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include<locale.h>
#include <inttypes.h>

#define MAX_TREE_HT 100
#define OFFSET 5
#define LENGTH (8-OFFSET)

#define OFFSETMASK ((1 << (OFFSET)) - 1)
#define LENGTHMASK ((1 << (LENGTH)) - 1)

#define GETOFFSET(k) (k >> LENGTH)
#define GETLENGTH(k) (k & LENGTHMASK)
#define OFFSETLENGTH(k,l) (k << LENGTH | l)
char *file_read(FILE *f, int *size);

struct token
{
    uint8_t offset_leng;
    char chr;
};

static inline int prefix_leng(char *s1, char *s2, int lim)
{
    int leng = 0;

    while (*s1++ == *s2++ && leng < lim)
    {
        leng++;
    }
    return leng;
}

static inline void lz77memcpy(char *s1, char *s2, int size)
{
    while (size--)
    {
        *s1++ = *s2++;

    }
}

char *decode(struct token *tokens, int numTokens, int *pcbDecoded)
{
    int cap = 1 << 3;

    *pcbDecoded = 0;

    char *decoded = malloc(cap);

    int i;
    for (i = 0; i < numTokens; i++)
    {
        int offset = GETOFFSET(tokens[i].offset_leng);
        int len = GETLENGTH(tokens[i].offset_leng);
        char chr = tokens[i].chr;

        if (*pcbDecoded + len + 1 > cap)
        {
            cap = cap << 1;
            decoded = realloc(decoded, cap);
        }

        if (len > 0)
        {
            lz77memcpy(&decoded[*pcbDecoded], &decoded[*pcbDecoded - offset], len);
        }

        *pcbDecoded += len;

        decoded[*pcbDecoded] = chr;

        *pcbDecoded = *pcbDecoded + 1;
    }

    return decoded;
}

struct token *encode(char *text, int limit, int *numTokens)
{
    FILE *f;
    int metin_boyutu = 6;
    char *kaynak_metin = "aaaaaaaa";

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }
    int el=1,h=0;
    char *mencode;
    mencode =(char *) malloc(el * sizeof(char));
    int cap = 1 << 3;

    int _numTokens = 0;

    struct token t;

    char *lookahead, *search;

    struct token *encoded = malloc(cap * sizeof(struct token));

    for (lookahead = text; lookahead < text + limit; lookahead++)
    {
        search = lookahead - OFFSETMASK;

        if (search < text)
        {
            search = text;
        }

        int max_len = 0;

        char *max_match = lookahead;

        for (; search < lookahead; search++)
        {
            int len = prefix_leng(search, lookahead, LENGTHMASK);

            if (len > max_len)
            {
                max_len = len;
                max_match = search;
            }
        }
        if (lookahead + max_len >= text + limit)
        {
            max_len = text + limit - lookahead - 1;
        }
        t.offset_leng = OFFSETLENGTH(lookahead - max_match, max_len);
        lookahead += max_len;
        t.chr = *lookahead;
        mencode[h]=t.chr;
        el++;
        h++;
        mencode = (char *) realloc(mencode, 2*el * sizeof(char));

        if (_numTokens + 1 > cap)
        {
            cap = cap << 1;
            encoded = realloc(encoded, cap * sizeof(struct token));
        }

        encoded[_numTokens++] = t;

    }

    if (numTokens)
    {
        *numTokens = _numTokens;
        printf("\n Kac karakter :%d token sayisi : %d --\n ",limit,*numTokens);
    }
    int eleman_sayisi=1;
    char *arr2;
    arr2 =(char *) malloc(eleman_sayisi * sizeof(char));

    int i=0;
    int j=0;

    int k,a=1,m=1,v=0;


    while(kaynak_metin[a]!='\0')
    {
        m++;
        a++;
    }
    int kackerecikti[a];
    for(i=0; i<a; i++)
    {
        kackerecikti[i]=0;
    }

    for(i=0; i<m; i++)
    {
        if(kackerecikti[i]==0)
        {
            for(j=i+1; j<m; j++)
            {
                if(kaynak_metin[j]==kaynak_metin[i])
                {
                    kackerecikti[i]++;
                    kackerecikti[j]++;
                }
            }

            arr2[v]=kaynak_metin[i];
            eleman_sayisi++;
            arr2 = (char *) realloc(arr2, 2*eleman_sayisi * sizeof(char));
            v++;
        }
    }

    for(i=0; i<m; i++)
    {
        kackerecikti[i]+=1;
    }
    for(i=0; i<h; i++)
    {
        printf("\n--%c--",mencode[i]);
    }



    int size2=sizeof(char)*eleman_sayisi;

    HuffmanCodes(arr2, kackerecikti, size2,mencode);

    return encoded;
}

char *file_read(FILE *f, int *size)
{
    char *content;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    content = malloc(*size);
    fseek(f, 0, SEEK_SET);
    fread(content, 1, *size, f);
    return content;
}

struct MinHeapNode
{
    char data;

    unsigned freq;

    struct MinHeapNode *left, *right;
};

struct MinHeap
{
    unsigned size;

    unsigned capacity;

    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp
        = (struct MinHeapNode*)malloc
          (sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap
        = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
        = (struct MinHeapNode**)malloc(minHeap->
                                       capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)

{

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)

{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
            freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
            freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
        = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap* minHeap,
                   struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int arr[], int n)
{
    FILE *f;
    f=fopen("DEFLATE.txt","a");

    int i;
    for (i = 0; i < n; ++i){
        printf("%d", arr[i]);
        fprintf(f,"%d", arr[i]);
 }
    printf("\n");
}

int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)

{
    struct MinHeapNode *left, *right, *top;

    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap))
    {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}
int toplamboyut;

void printArr2(int arr[], int n)
{
    FILE *f;
    f = fopen("DEFLATE.txt", "a");

    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%d", arr[i]);
        fprintf(f,"%d", arr[i]);
        toplamboyut+=1;
    }

}

//METNÝ YAZDIRIR
int kacinci=0;

void printTxt(struct MinHeapNode* root, int arr[], int top,int i,char mencode[])

{
    FILE *f;
    f = fopen("DEFLATE.txt", "a");


    if (root->left)
    {
        arr[top] = 0;
        printTxt(root->left, arr, top + 1,i,mencode);
    }

    if (root->right)
    {
        arr[top] = 1;
        printTxt(root->right, arr, top + 1,i,mencode);
    }
    if(mencode[i]==root->data)
    {
        if (isLeaf(root))
        {
            if(i==0){
                fprintf(f,"\n ");
            }

            printArr2(arr, top);
        }
    }
}

// Karakterlerin Encode halleri burada

void printCodes(struct MinHeapNode* root, int arr[], int top)
{
    FILE *f;
    f = fopen("DEFLATE.txt", "a");
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root))
    {

        printf("%c: ",root->data);
        fprintf(f,"\n%c: ",root->data);
        int yazilcak=root->data;
        //fwrite(yazilcak, sizeof(yazilcak), 1, f);

        printArr(arr, top);



    }
}

void HuffmanCodes(char data[], int freq[], int size,char mencode[])

{
    FILE *f;
    FILE *f2;
    f2 = fopen("DEFLATE.txt", "a");

    int metin_boyutu = 6;
    char *kaynak_metin = "aaaaaaaa";

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }
    int i;

    struct MinHeapNode* root
        = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;

    printf("\n Karakter kodlari :\n");
    printCodes(root,arr,top);
    printf("\n mn %s \n",mencode);
    fprintf(f2,"\n  %s \n",mencode);

    for(i=0; i<strlen(mencode); i++)
    {


        printTxt(root, arr, top,i,mencode);
    }


}
int main()
{
    FILE *f;
    FILE *f2;
    f2=fopen("DEFLATE.txt","wb");
    int metin_boyutu = 6, token_sayisi, decode_boyutu;
    char *kaynak_metin = "aaaaaaaa", *decoded_metin = "";
    struct token *encoded_metin;

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }
    printf("Kaynak : %s \n ",kaynak_metin);


    encoded_metin = encode(kaynak_metin, metin_boyutu, &token_sayisi);

    if (f = fopen("LZ77.txt", "wb"))
    {
        fwrite(encoded_metin, sizeof(struct token), token_sayisi, f);
        fclose(f);
    }

    decoded_metin = decode(encoded_metin, token_sayisi, &decode_boyutu);

    if (f = fopen("decoded.txt", "wb"))
    {
        fwrite(decoded_metin, 1, decode_boyutu, f);
        fclose(f);
    }
    printf("\n\n LZ77 ALGORITMASI:\n");
    printf("\n |-------------------------------------------------------------------|\n");
    printf(" |----- Orjinal Boyut: %d, Encode Boyutu: %d, Decode Boyutu: %d -----|\n", metin_boyutu, token_sayisi * sizeof(struct token), decode_boyutu);
    printf(" |-------------------------------------------------------------------|\n");
    int lz77boyut=token_sayisi * sizeof(struct token);
   // printf("Kaynak : %s \n",kaynak_metin);

    int kacbyte;
   //   Toplamboyut deflate algoritmasýnýn kaç bit olduðudur
   //printf("\DEFLATE BOYUT :%d",toplamboyut);

    if((toplamboyut%8)!=0){
    kacbyte= toplamboyut/8;
    kacbyte++;
    }
    else{
        kacbyte= toplamboyut/8;
    }
    printf("\n\n DEFLATE ALGORITMASI:\n");
    printf("\n |-------------------------------------------------------------------|\n");
    printf(" |-------------- Orjinal Boyut: %d , Encode Boyutu: %d --------------|\n", metin_boyutu,kacbyte, decode_boyutu);
    printf(" |-------------------------------------------------------------------|\n");

    printf("\n\n\n    SONUC:  \n");
    if(lz77boyut>kacbyte){
        printf("\tDeflate algoritmasi  LZ77 algoritmasindan daha iyi bir algoritmadir\n\n");
    }
    else if(lz77boyut<kacbyte){
        printf("LZ77 Algoritmasi Deflate algoritmasindan daha iyi bir algoritmadir\n\n");
    }


    return 0;
}
