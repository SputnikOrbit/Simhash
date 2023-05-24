#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<assert.h>
FILE* out;
//char *(*dic2)[1000][7];
//这是数组指针，我们要的是数组指针的指针数组
int array[10000];
char *array_words[10000];
int array_top = -1;
void clear_rear(char* str)
{
    int len = strlen(str)-1;
    while (len>=0&&!isalpha(str[len--]))
        str[len+1] = 0;
}

#define ALPHABET_SIZE 26
void array_append(char* str)
{
    int len = strlen(str);
    array_top++;
    array[array_top] = 1;
    array_words[array_top] = (char*)malloc(sizeof(char)*len+1);
    assert(memcpy((array_words[array_top]),str,len+1));
    fprintf(out,"%s\n",array_words[array_top]);
}
typedef struct trie_node
{
    int count;   // 记录该节点代表的单词的个数
    struct trie_node *children[ALPHABET_SIZE]; // 各个子节点 
}trie_node;

trie_node* create_trie_node()
{
    trie_node* pNode = (trie_node*)malloc(sizeof(trie_node));
    pNode->count = 0;
    for(int i=0; i<ALPHABET_SIZE; ++i)
        pNode->children[i] = NULL;
    return pNode;
}

void trie_insert(trie_node* root, char* key)
{
    trie_node* node = root;
    char* p = key;
    int flag = 0;
    while(*p)
    {
        if(node->children[*p-'a'] == NULL)
        {
            if(!flag)
            {
            array_append(key);
                flag = 1;
            }
            node->children[*p-'a'] = create_trie_node();
        }
        node = node->children[*p-'a'];
        ++p;
    }
    node->count += 1;
}

/**
 * 查询：不存在返回0，存在返回出现的次数
 */ 
int trie_search(trie_node* root, char* key)
{
    trie_node* node = root;
    char* p = key;
    while(*p && node!=NULL)
    {
        node = node->children[*p-'a'];
        ++p;
    }
    if(node == NULL)
        return 0;
    else
        return node->count;
}
int main()
{
    time_t start,end;
    start = clock();
    trie_node* root = create_trie_node();
    FILE* in = fopen("testbase.txt","r");
    out = fopen("testbase_out.txt","w");

    char word[90];
    while (fscanf(in,"%s",word)!=-1)
    {
        trie_insert(root, word);
    }
    char mtarget[] = "mailing";
    // for (int i = 0; i < 100000; i++)
    // trie_search(root,mtarget);
    
    
    //printf("%d\n",sizeof(dic2));
//    set_up();
    fclose(in);
    fclose(out);
    end = clock();
    printf("Time: %f\n",(double)(end-start)/CLOCKS_PER_SEC);
    return 0;      
}

//事实证明FGETS在读取完一行东西后不会把结尾的东西丢掉
