#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct                           // cau truc tree để lưu data
{
    char *word;
    int count;
    char *line;
}Data;

typedef struct nodetype{
    Data key;
    struct nodetype *right,*left;
}node;


typedef struct nodetype *tree;
// insert tree 
void insertnode(tree *root,Data x)
{
    if((*root)==NULL)
    {
        *root=(node*)malloc(sizeof(node));
        (*root)->key=x;
        (*root)->right=NULL;
        (*root)->left=NULL;
    }
    else if(strcmp(x.word,((*root)->key).word)>0) insertnode(&(*root)->right,x);
    else if(strcmp(x.word,((*root)->key).word)<0) insertnode(&(*root)->left,x);
    else{
        ((*root)->key).count++;
        if(strstr(((*root)->key).line,x.line)==NULL) strcat(((*root)->key).line,x.line);
    }
}
// search tree 
tree search(tree root,char *x){
    if(root==NULL) return NULL;
    else if(strcmp(x,(root->key).word)>0) search(root->right,x);
    else if(strcmp(x,(root->key).word)<0) search(root->left,x);
    else return root;
}

// string chữ hoa thành chữ thường
char * toLowercaseString(char *str){
    int len = strlen(str);
    char * strLowerCase = (char*) malloc(sizeof(char)* len);
    for(int i=0;i<=strlen(str);i++){
        if(str[i]>=65&&str[i]<=90){
            strLowerCase[i]=str[i]+32;
        }else {
            strLowerCase[i]=str[i];
        }        
        
    }
    return strLowerCase;
}
char *ltrim(char *str, const char *seps){
    size_t totrim;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0) {
        size_t len = strlen(str);
        if (totrim == len) {
            str[0] = '\0';
        }
        else {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
    return str;
}
char *rtrim(char *str, const char *seps){
    int i;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}
// xóa khoảng trắng 2 đầu
char *trim(char *str){
    return ltrim(rtrim(str, NULL), NULL);
}
// check str rỗng
int isEmpty(char *str){
    if (str[0]=='\n' ||str[0]=='\0' || str[0]=='\t')
        return 1;
    return 0;
}
int hasDigit(char *str){
    int len = strlen(str);
    for(int i =0; i < len;i++){
        if (isdigit(str[i])){
            return 1;
        }
        
    }
    return 0;
}
int isHasDot(char *str){
    int len = strlen(str);
    if (str[len -1]=='.')    {
        return 1;
    }
    return 0;
}
char* removeDot(char *str){
    int len = strlen(str);
    if (isHasDot(str)){
        str[len -1]='\0';
    }
    return str;
}
// check chữ cái đầu là hoa 
int isProperNoun(char *str){
    if (!isEmpty(str) && str[0]>= 'A' && str[0] <= 'Z'){
        return 1 ;
    }
    return 0;
}
// so sánh 2 string
int compareString(char* word1 ,char* word2){
    return strcmp(toLowercaseString(trim(word1)),toLowercaseString(trim(word2)));
}
// check stop word
int isStopWord(char** stopWordList,int stopListSize, char* word){
    for (int i=0;i< stopListSize; i++){
        if (compareString(stopWordList[i],word) == 0) return 1;
    }
    return 0;
}
FILE * openFile(char *filename, char * type){
    FILE *fp;
    fp = fopen(filename, type);
    if (fp ==NULL){
        printf("Could not open file %s", filename);
    }
    return fp;
}
// insert data to tree by string word 
void insertDataString(char * token ,int lineNumer, int columNum, tree *treeWord){
    tree data = search(*treeWord,token);
        if (data !=NULL){
            char *str =(char*) malloc(sizeof(char)*20) ;
            sprintf(str,",(%d, %d) ",lineNumer, columNum);
            strcat((data->key).line, str);
            (data->key).count = (data->key).count+1;
            free(str);
        }else{
            Data words ;
            words.word = (char*) malloc(sizeof(char)*50);
            words.line = (char*) malloc(sizeof(char)*10000);
            strcpy(words.word, token);
            words.count = 1 ;
            sprintf(words.line,",(%d, %d) ",lineNumer, columNum);
            insertnode(treeWord,words);
         }
}
// doc file text 
void readFileText(char* filename,char** stopWordList,int stopListSize,tree *treeWord)
{
    FILE *fp;
    fp = openFile(filename, "r");
    if (fp == NULL) return;
    char *strLine = (char*) malloc(sizeof(char)*200);
    char *token = (char*) malloc(sizeof(char)*50);
    const char divider[50] = " \n,()-;'`;\"!?:*";
    int lineNumer = 0;
    int isStartSentence =0;
    while (fgets(strLine,199,fp)!= NULL){
        // doc string theo dòng sau đó tách thành các từ -> check điều kiện rồi insert tree 
        lineNumer++;
        token = strtok(trim(strLine), divider);
        int colNumber = 0;
        while( token != NULL ) {
            colNumber++;
                if (isStartSentence==0){
                    if (isHasDot(token)){
                        isStartSentence = 1;
                    }
                    token = removeDot(token);
                }else{
                    isStartSentence = 0;
                    if(isStopWord(stopWordList,stopListSize,token)==0 && hasDigit(token) ==0 && isEmpty(token) ==0){
                        insertDataString(toLowercaseString(token),lineNumer,colNumber, treeWord);
                    }
                    continue;
                }
                if (isStopWord(stopWordList,stopListSize,token)==0 && hasDigit(token) ==0 && isEmpty(token) ==0 ){
                    insertDataString(toLowercaseString(token),lineNumer,colNumber,treeWord);
                }

            token = strtok(NULL, divider);
            
        }
       
    }
    
    fclose(fp); 
}
// doc danh sach stop word tu file
char ** getListStopWorld(char *filename,int * numWord){
    char *str = (char*) malloc(sizeof(char)*49);
    char** a = (char **)malloc(50 * sizeof(char *));
    for (int i = 0; i < 50; i++){
        a[i] = (char *)malloc(50 * sizeof(char));
    }
    FILE *fp;
    fp = openFile(filename, "r");
    if (fp ==NULL)  return NULL;
    int i = 0;
    while(!feof(fp)){
        fscanf(fp ,"%s\n", str);
        strcpy(a[i], str);
        (i)++;
    }
    *numWord = i;
    free(str);
    fclose(fp);
    return a;
}
// in tree word ra man hinh 
void displayTree(tree root){
    if (root!=NULL) {
        displayTree(root->left);
        printf("\n%-20s%-2i%s",(root->key).word,(root->key).count,(root->key).line);
        displayTree(root->right);
    }
}
void main (void ){
    char* stopWordFileName = "stopw.txt";
    char* textFileName = "vanban.txt";
    int* numStopWord = malloc(sizeof(int));
    char ** listStopWord = getListStopWorld(stopWordFileName,numStopWord);
    tree treeWord=NULL ;
    readFileText(textFileName,listStopWord,*numStopWord,&treeWord);
    displayTree(treeWord);

}

//DinhQuocCuong_20184054
