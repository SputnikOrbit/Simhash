#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
FILE* ARTICLE_FILE,* STWORD_FILE,* HASH_FILE,*RESULT_FILE,*SAMPLE_FILE;

typedef struct node
{
    char* name_code;
    char* fingerprint;
}articleNode;

char *hash[10005];

articleNode *sample;
articleNode *articles[16001];
char *sample_name;

char *stopWord[10000];
int stopWordNum = 0;

int N, M;


void clear_rear(char* str)
{
    int len = strlen(str)-1;
    while (len>=0&&!isalpha(str[len--]))str[len+1] = 0;}
void get_hash()
{for(int i = 0;i<N;i++){
                hash[i] = (char*)malloc(sizeof(char)*(M+1));
                assert(fgets(hash[i],M+1,HASH_FILE));}}

void get_stw()
{
    int stopword_limit = 10000;
    char word[90];
    memset(word,0,sizeof(word));
    while (fscanf(STWORD_FILE,"%s",word) != EOF)
    {
        clear_rear(word);
        stopWord[stopWordNum] = (char*)malloc(sizeof(char)*(strlen(word)+1));
        strcpy(stopWord[stopWordNum],word);
        //fprintf(RESULT_FILE,"%s\n",stopWord[stopWordNum]);
        stopWordNum++;
    }
}
int Mybsearch(char **arr,char *target,int len)
{
    int left = 0,right = len-1;
    int mid;
    while(left<=right)
    {
        mid = left+(right-left)/2;
        if(strcmp(arr[mid],target)==0)
        {
            //printf("find it at %d\n",mid);
            return mid;
        }
        else if(strcmp(arr[mid],target)<0)
            left = mid+1;
        else
            right = mid-1;
    }
    return -1;
}
// USAGE:int target_index = Mybsearch(stw,target,stopWordNum);
void get_sample_name()
{
    char line[1024];
    memset(line,0,sizeof(line));
    fscanf(SAMPLE_FILE,"%s",line);
        int len = strlen(line);
        sample_name = (char*)malloc(sizeof(char)*(len+1));
        memcpy(sample_name,line,len+1);
        fprintf(RESULT_FILE,"%s\n",sample_name);
}
void get_article_FGP(FILE* STREAM, articleNode* article)
{
    article = (articleNode*)malloc(sizeof(articleNode));
    static int times = 5;
    article->fingerprint = (char*)malloc(sizeof(char)*(M+1));
    char line[1024];
    fscanf(STREAM,"%s",line);
    int len = strlen(line);
    article->name_code = (char*)malloc(sizeof(char)*(len+1));
    memcpy(article->name_code,line,len+1);

    printf("%s\n",article->name_code);//testing///////////////////////////////////////////////////////

    char word[90];
    int word_len = 0;
    memset(word,0,sizeof(word));
    char ch;
    fscanf(STREAM,"%c",&ch);
    while(ch!='\f')
    {
        if (isalpha(ch))
        {
            word_len = 0;
        memset(word,0,sizeof(word));
        word[word_len++] =tolower(ch);
        fscanf(STREAM,"%c",&ch);
        while (isalpha(ch))
        {
            word[word_len++] = tolower(ch);
            fscanf(STREAM,"%c",&ch);       
        }
        word[word_len] = 0;

        int len = strlen(word);
        int tag = Mybsearch(stopWord,word,stopWordNum);
            if(tag==-1&&times)
            {
                printf("%s\n",word);
                times--;//testing/////////////////////////////////////////////////////////////////////
            }
        }
        if (times == 0)
        {
            return;
        }
        fscanf(STREAM,"%c",&ch);
    }
    
}
void burn_down(articleNode* article)
{
    int len = strlen(article->name_code);
    for (int i = 0; i < len; i++)
    {
        free(article->name_code);
        article->name_code++;
    }
    article->name_code = NULL;
    len = strlen(article->fingerprint);
    for (int i = 0; i < len; i++)
    {
        free(article->fingerprint);
        article->fingerprint++;
    }
    article->fingerprint = NULL;
    free(article);
    article = NULL;
}


int main(int argc, char const *argv[])
{
    assert(ARTICLE_FILE = fopen("article.txt","r"));
    assert(STWORD_FILE = fopen("stopwords.txt","r"));
    assert(HASH_FILE = fopen("hashvalue.txt","r"));
    assert(RESULT_FILE = fopen("result.txt","w"));
    assert(SAMPLE_FILE = fopen("sample.txt","r"));//fileopen confirmed

    //N = atoi(argv[1]);
    //M = atoi(argv[2]);//cmd confirmed
    //scanf("%d %d",&N,&M);
    N = 5;
    M = 12;//fixed configure !

    get_hash();//confirmed

    get_stw();//confirmed


    //get base article fingerprints
    get_article_FGP(ARTICLE_FILE, articles[0]);

    //get_sample
        //get sample name/////////////////////////////////////////////////////////////////////////////////////
        // int flag = 0;
        // char tmp = 0;
        // do
        // {
        //     get_sample_name();
            

        //     scanf("%c",&tmp);
        // }while (tmp =='\f');
        
        // get_sample_name();//confirmed
        // //get sample fingerprint
        // get_sample_FGP();
        

    // int stopword_limit = 1000;
    // char *word[90];
    // while (fgets(stopWord[stopWordNum],100,STWORD_FILE) != NULL)
    // {
    //     stopWordNum++;
    //     if (stopWordNum == stopword_limit)
    //     {
    //         stopword_limit += 1000;
    //         stopWord = (char**)realloc(stopWord,stopword_limit*sizeof(char*));
    //     }
        
        
    // }

    
    
    //qsort
    

    fclose(ARTICLE_FILE);
    fclose(STWORD_FILE);
    fclose(HASH_FILE);
    fclose(RESULT_FILE);
    fclose(SAMPLE_FILE);
    return 0;
}