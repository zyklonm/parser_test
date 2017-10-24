#include    <iostream>
#include    <fstream>
#include    <string>
#include    <locale.h>
#include    <stdlib.h>
#include    <time.h>
#include    "/home/moon/Downloads/wiki_test/wiki_parser/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

#define SEPARATION_WORD_NUM 100000000

#define WORD_LENGTH_ONE 1
#define WORD_LENGTH_TWO 2
#define WORD_LENGTH_THREE 3
#define WORD_LENGTH_FOUR 4
#define WORD_LENGTH_FIVE 5

char *word[SEPARATION_WORD_NUM];
int *word_same_num;
int word_num = 0;
int for_i =0;

int len_1;
int len_2;
int len_3;
int len_4;
int len_5;

bool same_word_checker()
{
    for( for_i = 0; for_i < word_num; for_i++ )
    {
	if(!strcmp(word[for_i], word[word_num]))
	{
	    //cout<<"What the Fxxx"<<endl;
	    *(word_same_num + for_i) += 1;
	    return true;
	}
    }
    return false;
}

void separation(char* strText,int mode)
{
    for( ; *strText != '\0'; strText++ )
    {
	if( word_num >= SEPARATION_WORD_NUM)
	{
	    return;
	}
	len_1 = mblen(&strText[0],MB_CUR_MAX);
	len_2 = mblen(&strText[len_1],MB_CUR_MAX);
	len_3 = mblen(&strText[len_1+len_2],MB_CUR_MAX);
	len_4 = mblen(&strText[len_1+len_2+len_3],MB_CUR_MAX);
	len_5 = mblen(&strText[len_1+len_2+len_3+len_4],MB_CUR_MAX);
	if( len_1 > 1 )
	{
	    switch(mode)
	    {
		case WORD_LENGTH_ONE:
		{
		    word[word_num] = (char*)malloc(sizeof(char)*len_1);
		    for( for_i = 0; for_i < len_1; for_i ++ )
		    {
			*((*(word + word_num)) + for_i) = strText[for_i];
		    }
		    *((*(word+word_num))+len_1) = '\0';
		    
		    if(same_word_checker())
		    {
			free(word[word_num]);
		    }
		    else
		    {
			word_num++;
		    }
		    strText += (len_1-1);
		    break;
		}

		case WORD_LENGTH_TWO:
		{
		    if( len_2 > 1 )
		    {
			word[word_num] = (char*)malloc(sizeof(char)*(len_1+len_2));
			for( for_i = 0; for_i < (len_1+len_2); for_i++)
			{
			    *((*(word + word_num)) + for_i) = strText[for_i];
			}
			*((*(word+word_num))+len_1+len_2) = '\0';

			if(same_word_checker())
			{
			    free(word[word_num]);
			}
			else
			{
			    word_num++;
			}
			strText += (len_1-1);
		    }
		    break;
		}
		case WORD_LENGTH_THREE:
		{
		    if( len_2 > 1 && len_3 > 1)
		    {
			word[word_num] = (char*)malloc(sizeof(char)*(len_1+len_2+len_3));
			for( for_i = 0; for_i < (len_1+len_2+len_3); for_i++)
			{
			    *((*(word + word_num)) + for_i) = strText[for_i];
			}
			
			*((*(word+word_num))+len_1+len_2+len_3) = '\0';

			if(same_word_checker())
			{
			    free(word[word_num]);
			}
			else
			{
			    word_num++;
			}

			strText += (len_1 - 1);
		    }
		    break;
		}
		default:
		    break;
	    }
	}
    }
}

int main()
{
    clock_t start, end, sub_start, sub_end;
    start = clock();
    word_same_num = (int*)malloc(sizeof(int)*SEPARATION_WORD_NUM);
    for( for_i = 0; for_i <= SEPARATION_WORD_NUM; for_i++)
    {
	*(word_same_num+for_i) = 1;
    }
    setlocale( LC_CTYPE, "" );

    XMLDocument xml;
    xml.LoadFile("mini_test.xml");

    XMLElement *pageElm = xml.FirstChildElement("page");

//temp code_start
    int n = 0;
    int num = 0;
//temp code end
    while( pageElm != NULL )
    {
	char *title_string = (char*)(pageElm->FirstChildElement("title")->GetText());
	char *category_string = (char*)(pageElm->FirstChildElement("category")->GetText());
	char *text_string = (char*)(pageElm->FirstChildElement("text")->GetText());

//	cout<<"Title : " <<title_string<<endl;
//	cout<<"Category : "<<category_string<<endl;
//	cout<<"Text : "<<text_string<<endl;

//temp code_start
	cout<<"page num : "<<n<<endl;
	cout<<"word num : "<<word_num<<endl;
	sub_start = clock();	
	num = word_num;
//temp_code_end

	separation(text_string, WORD_LENGTH_THREE);

//temp_code_start	
	sub_end = clock();
	cout<<"plus word : " << word_num - num<<endl;
	printf("end : %f(sec) \n\n",(float)(sub_end-sub_start)/(CLOCKS_PER_SEC));
//temp_code_end
	pageElm = (XMLElement *)(pageElm->NextSibling());
	n += 1;
    }
    int i = 0;
    for( i = 0; i < word_num; i++)
    {
	cout<<word[i]<<"  :  "<<*(word_same_num+i)<<endl;
    }
    end = clock();
    cout<<"Total : " << word_num<<endl;
    printf("Run Time : %.3lf(sec) \n",(float)(end-start)/(CLOCKS_PER_SEC));
    return 0;
}
