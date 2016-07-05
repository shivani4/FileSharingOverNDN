#include <iostream>
#include <string>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <fstream>
#include<time.h>
#define MAX_SEG 32
#define node_size 5
using namespace std;
char timer[300];
int ot=0;
//using namespace ndn;
char producer_reg[100][100];
int number_of_prod_files=0; 
char consumer_reg[100][100];
int number_of_cons_files=0;
//int max_seg_consumer=0; 
const char HEX2DEC[256] = 
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
    
    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};
class Consumer
{
public:

  void start_c()
  {
    memset(&timer,0,sizeof(timer));
    printf("Entering Start of Consumer\n");
    int k=0;
    for(k=0;k<number_of_cons_files;k++)
    { 
     strcat(timer,"\n");
     strcat(timer,consumer_reg[k]);
     char buf[16];
     snprintf(buf, 16, "%lu", time(NULL));
     strcat(timer,"  Begin:  ");
     strcat(timer,buf); 
     strcat(timer,"  ");
    ndn::Interest request(ndn::Name(consumer_reg[k]));
    m_face.expressInterest(request,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));
     
    }
    m_face.processEvents();
  }

protected:

  virtual void
  onData(const ndn::Interest& interest, const ndn::Data& data)
  {
    int dollarfound=0;
    int hashfound=0; 
    int i=0;
    cout << data.getName().size() << endl;
    int comp_size=data.getName().size();
    string last_comp=data.getName().get(comp_size-1).toUri();
    string check=UriDecode(last_comp);
    cout<<check<<endl;
    cout<<"Last component is "<<last_comp<<endl;
    std::size_t found = check.find("#");
    if (found!=std::string::npos)
    {
       hashfound=1;
    }
    found = check.find("$");
    if (found!=std::string::npos)
    {
       dollarfound=1;
    }
    printf("DollarFound:%d and HashFound:%d\n",dollarfound,hashfound); 
    std::cout << "Received Data " << data << std::endl;

    const ndn::Block& payload = data.getContent();

    std:: cout << "payload: ";
    std::cout.write(reinterpret_cast<const char*>(payload.value()), payload.value_size());
    std:: cout << std::endl;
 
    if(dollarfound==1)
    {
      printf("Inside Dollar Found\n");      
      const char *sending=reinterpret_cast<const char*>(payload.value()); 
      string received=string(sending);
      int  max_seg_consumer=stoi(received);
      printf("Maximum Number of Segments is =%d\n",max_seg_consumer);
      //Write code to express interest for the 0th segment   
      string seg_req_0=UriDecode(data.getName().toUri());
      seg_req_0.pop_back();
      seg_req_0.append(to_string(max_seg_consumer));
      seg_req_0.append("/#0");
      printf("String for 0th segment is :");    
      cout<<seg_req_0<<endl;
      char str_send[2][1000];
      memset(&str_send,0,sizeof(str_send));
      strcpy(str_send[0], seg_req_0.c_str());
      cout<<"String being sent in $ is:"<<str_send[0]<<endl;
      const ndn::Name name1(str_send[0]);
      ndn::Interest request(name1);
      m_face.expressInterest(request,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));
//    m_face.processEvents();
}
    if(hashfound==1){
    printf("Inside Hash Fund\n");
    //Write code to print to the file and ask the next segment
    string filename;
    for(i=0;i<comp_size-2;i++)
   {
        string mid=data.getName().get(i).toUri();
        string checking=UriDecode(mid);
        filename = filename + '_' +mid;
   }
   char node_name[node_size];
   gethostname(node_name,node_size);
   string node_n=string(node_name);
   node_n.append("-");
   node_n=node_n + filename;
   cout<<node_n<<endl;

   FILE *fp1=fopen(node_n.c_str(),"a");
   fseek(fp1, 0L, SEEK_END);
   int start_ind=0;
   start_ind=stoi(check.substr(1));
    const char *sending=reinterpret_cast<const char*>(payload.value());
    int start1=start_ind*MAX_SEG;
    fseek(fp1,start1,SEEK_SET);
    int just_checking=0;
    if (!(just_checking=fwrite(sending,1,(int)payload.value_size(),fp1))) {
    fprintf(stderr, "Failure");
    //perror(???)  I sometimes see code that says perror here and I don't know 
    //exactly what this does.
}
    printf("Just checking.......%d\n",just_checking);
    //fwrite(sending,1,(int)payload.value_size(),fp1);

//    fwrite(sending,1,payload.value_size(),fp1);
    fclose(fp1);
  /* int start_ind=stoi(check.substr(1));
   std::ofstream ofs;
   ofs.open (node_n, std::ofstream::out | std::ofstream::app);
   ofs.write(reinterpret_cast<const char*>(payload.value()), payload.value_size());
   ofs.close();*/
    start_ind++;
 //   printf("Maximum segment count inside the hash function before the condition is:%d and value of start is: %d\n",max_seg_consumer,start_ind);
   int max_seg_consumer=stoi(UriDecode(data.getName().get(comp_size-2).toUri())); 
   if(start_ind<max_seg_consumer)
    {
      printf("Inside the if consition for max segments\n"); 
      string seg_req=UriDecode(data.getName().toUri());
      size_t found = seg_req.find_last_of("#");
      seg_req=seg_req.substr(0,found+1);
      seg_req.append(to_string(start_ind));
      char str_send[5][1000];
      memset(&str_send,0,sizeof(str_send));
      strcpy(str_send[0], seg_req.c_str());
      cout<<"String that is being sent in ####"<<str_send<<endl;
      const ndn::Name name1(str_send[0]);
      ndn::Interest request(name1);
      m_face.expressInterest(request,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));
    }
    else
   {
       char buf[16];
       snprintf(buf, 16, "%lu", time(NULL)); 
       strcat(timer,"\n");
       char tab2[100];
       memset(&tab2,0,sizeof(tab2));
       strcpy(tab2, node_n.c_str());
       strcat(timer,tab2);
       strcat(timer," End: ");
       strcat(timer,buf);
       strcat(timer,"\n");
       FILE *fp3;
       fp3=fopen("logs_shivani.txt","a"); 
       fwrite(timer,1,sizeof(timer),fp3);
       fclose(fp3);
       memset(&timer,0,sizeof(timer));
    }
    
 }

 }

  virtual void
  onTimeout(const ndn::Interest& interest)
  {
    std::cout << "Interest timed out" << interest << std::endl;
    /*m_face.expressInterest(interest,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));
    ot++;
    if(ot==32500)*/
     exit(0);
    
  }
virtual std::string UriDecode(const std::string & sSrc)
{
   // Note from RFC1630: "Sequences which start with a percent
   // sign but are not followed by two hexadecimal characters
   // (0-9, A-F) are reserved for future extension"

   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
   const int SRC_LEN = sSrc.length();
   const unsigned char * const SRC_END = pSrc + SRC_LEN;
   // last decodable '%'
   const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

   char * const pStart = new char[SRC_LEN];
   char * pEnd = pStart;

   while (pSrc < SRC_LAST_DEC)
   {
      if (*pSrc == '%')
      {
         char dec1, dec2;
         if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
            && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
         {
            *pEnd++ = (dec1 << 4) + dec2;
            pSrc += 3;
            continue;
         }
      }

      *pEnd++ = *pSrc++;
   }

   // the last 2- chars
   while (pSrc < SRC_END)
      *pEnd++ = *pSrc++;

   std::string sResult(pStart, pEnd);
   delete [] pStart;
   return sResult;
}
protected:
  const ndn::Name m_prefix;
  ndn::Face m_face;
};



class Producer
{

public:  

 void start()
  {
    printf("Entering start\n");
    int k=0;
     for(k=0;k<number_of_prod_files;k++){
     m_face.setInterestFilter(ndn::InterestFilter(producer_reg[k]),
                             bind(&Producer::onInterest, this, _1, _2),
                             bind(&Producer::onRegisterSuccess, this, _1),
                             bind(&Producer::onRegisterFailure, this, _1, _2));

}
    m_face.processEvents();

  }

  virtual void
  onRegisterSuccess(const ndn::Name& prefix)
  {
    std::cout << "Successfully registered " << prefix << std::endl;
  }

  virtual void
  onRegisterFailure(const ndn::Name& prefix, const std::string& reason)
  {
    std::cout << "Failed to register prefix " << prefix << ": " << reason << std::endl;
  }

protected:

  virtual void
  onInterest(const ndn::InterestFilter& filter, const ndn::Interest& interest)
  {
   
    int dollarfound=0;
    int hashfound=0;
    printf("This doesn't work"); 
    std::cout << "Checking if this works " << interest.getName().size() << std::endl;
    int comp_size=interest.getName().size();
    string last_comp=interest.getName().get(comp_size-1).toUri();
    string check=UriDecode(last_comp);
    cout<<check<<endl;
    cout<<"Last component is "<<last_comp<<endl;
    std::size_t found = check.find("#");   
    if (found!=std::string::npos) 
    {
       hashfound=1;
    }
    found = check.find("$");
    if (found!=std::string::npos)
    {
       dollarfound=1;
    }
    printf("DollarFound:%d and HashFound:%d\n",dollarfound,hashfound);
    std::cout << "Received Interest " << interest << std::endl;

     
    //Creating the file name
    std::shared_ptr<ndn::Data> response(std::make_shared<ndn::Data>(interest.getName()));

 
    if(dollarfound==1)
    {

     int i=0;
     string filename;
     for(i=0;i<comp_size-1;i++)
     { 
        string mid=interest.getName().get(i).toUri();
        string checking=UriDecode(mid);
        filename = filename + '_' +mid;
    }
     char node_name[node_size];
     gethostname(node_name,node_size);
    string node_n=string(node_name);
    node_n.append("-");
    node_n=node_n + filename;
    cout<<node_n<<endl;

   // char data[1000];
    FILE *fp1=fopen(node_n.c_str(),"r+");
    fseek(fp1, 0L, SEEK_END);
    int number_of_segments=0;
    int size= ftell(fp1);



       if(size%MAX_SEG!=0)
      {
       	number_of_segments=(size/MAX_SEG)+1;
      }
      else
      {
       	number_of_segments=(size/MAX_SEG);
      }
      string data1=to_string(number_of_segments);
    //  string data2='$'+data1;
      printf("Number of Segments is =%d\n",number_of_segments);
      response->setContent(reinterpret_cast<const uint8_t*>(data1.c_str()),
                         data1.size());
      fclose(fp1);
     }
    if(hashfound==1){
     int i=0;
     string filename;
     for(i=0;i<comp_size-2;i++)
     {
        string mid=interest.getName().get(i).toUri();
        string checking=UriDecode(mid);
        filename = filename + '_' +mid;
     }
     char node_name[node_size];
     gethostname(node_name,node_size);
    string node_n=string(node_name);
    node_n.append("-");
    node_n=node_n + filename;
    cout<<node_n<<endl;

    char data[1000];
  //  char data[32];
    FILE *fp1=fopen(node_n.c_str(),"r+");
    fseek(fp1, 0L, SEEK_END);
  //  int number_of_segments=0;
    //int size= ftell(fp1);
    int start=0;
    start=stoi(check.substr(1))*MAX_SEG;
    fseek(fp1,start,SEEK_SET);
    memset(&data,0,sizeof(data));
    size_t check=fread(data,MAX_SEG,1,fp1);
    
    if(check==0)
     printf("Error\n");
 //   printf("Data: %s\n",data);
    string msg=string(data);
    response->setContent(reinterpret_cast<const uint8_t*>(data),
                         MAX_SEG);
  //  printf("Size of the outgoing message is:%d",msg.size());
    cout<<msg.c_str()<<endl;
   }
    m_keyChain.sign(*response);
    m_face.put(*response);
  }
virtual std::string UriDecode(const std::string & sSrc)
{
   // Note from RFC1630: "Sequences which start with a percent
   // sign but are not followed by two hexadecimal characters
   // (0-9, A-F) are reserved for future extension"
 
   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
   const int SRC_LEN = sSrc.length();
   const unsigned char * const SRC_END = pSrc + SRC_LEN;
   // last decodable '%' 
   const unsigned char * const SRC_LAST_DEC = SRC_END - 2;
 
   char * const pStart = new char[SRC_LEN];
   char * pEnd = pStart;
 
   while (pSrc < SRC_LAST_DEC)
   {
      if (*pSrc == '%')
      {
         char dec1, dec2;
         if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
            && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
         {
            *pEnd++ = (dec1 << 4) + dec2;
            pSrc += 3;
            continue;
         }
      }
 
      *pEnd++ = *pSrc++;
   }
 
   // the last 2- chars
   while (pSrc < SRC_END)
      *pEnd++ = *pSrc++;
 
   std::string sResult(pStart, pEnd);
   delete [] pStart;
   return sResult;
}
protected:
 // const ndn::Name m_prefix;
  ndn::Face m_face;
  ndn::KeyChain m_keyChain;
};


int
main(int argc, char** argv)
{
 // char consumer_reg[100];
  int i=0;
  int flag_prod=0;
  int flag_cons=0;
  int check_flag=0;

  for(i=0;i<argc;i++)
  {
    if((strcmp(argv[i],"-p")==0)) 
      check_flag++;
    if((strcmp(argv[i],"-d")==0))
      check_flag++; 
  }
if(check_flag==1){
  for(i=1;i<argc;i++)
  {
	if((strcmp(argv[i],"-p")==0))
	{
		printf("In Producer\n");
		i=i+1;
		flag_prod=1;
		break;
	}
	if((strcmp(argv[i],"-d")==0))
	{
		printf("In Consumer\n");
		i=i+1;
		flag_cons=1;
		break;
	}
  }
  if(flag_prod==1)
{
	int j=0;
	  number_of_prod_files=argc-i;
          for(j=0;j<number_of_prod_files;j++)
          {
		strcpy(producer_reg[j],argv[i]);
		++i;
	  }
	  printf("Files in the Producer are as below\n");
          for(j=0;j<number_of_prod_files;j++)
	  {
		printf("%s\n",producer_reg[j]);
	  }
	  Producer p;
	  p.start();
          flag_prod=0;
 }
else  if(flag_cons==1)
 {
	int j=0;
        char string_to_send[800];
	  number_of_cons_files=argc-i;
          for(j=0;j<number_of_cons_files;j++)
          {
		memset(&string_to_send,0,sizeof(string_to_send));
                strcat(string_to_send,argv[i]);
                strcat(string_to_send,"/$");
		strcpy(consumer_reg[j],string_to_send);
		++i;
	  }
	  printf("Files in the Consumer are as below\n");
          for(j=0;j<number_of_cons_files;j++)
	  {
		printf("%s\n",consumer_reg[j]);
	  }


	Consumer c;
	c.start_c();
        flag_cons=0;
}
}
else if(check_flag==2)
{
  int p_index=0;
  int d_index=0;
  for(i=1;i<argc;i++)
  {
	 if((strcmp(argv[i],"-p")==0))
           p_index=i;
         if((strcmp(argv[i],"-d")==0))
           d_index=i;
         if(p_index!=0 && d_index!=0)
          break;
  }    	
  if(p_index<d_index && p_index!=0)
  {
    i=0;
    int j=0;
    for(j=p_index+1;j<d_index;j++)
    {
                strcpy(producer_reg[i],argv[j]);
                ++i;
     }
      number_of_prod_files=i;
      printf("Files in the Producer are as below\n");
      for(j=0;j<number_of_prod_files;j++)
      {
               printf("%s\n",producer_reg[j]);
      }
      i=0;
      char string_to_send[800];
      for(j=d_index+1;j<argc;j++)
      {
        memset(&string_to_send,0,sizeof(string_to_send));
        strcat(string_to_send,argv[j]);
        strcat(string_to_send,"/$");
        strcpy(consumer_reg[i],string_to_send);
        i++;
      }
      number_of_cons_files=i;
      printf("Files in the Consumer are as below\n");
      for(j=0;j<number_of_cons_files;j++)
      {
                printf("%s\n",consumer_reg[j]);
      }

  }	
  if(d_index<p_index && d_index!=0)
  {
   int j=0;
   i=0;
   char string_to_send[800];
   for(j=d_index+1;j<p_index;j++) 
   {
        memset(&string_to_send,0,sizeof(string_to_send));
        strcat(string_to_send,argv[j]);
        strcat(string_to_send,"/$");
        strcpy(consumer_reg[i],string_to_send);
        i++; 
   }
      number_of_cons_files=i;
      printf("Files in the Consumer are as below\n");
      for(j=0;j<number_of_cons_files;j++)
      {
                printf("%s\n",consumer_reg[j]);
      }

     i=0;
     for(j=p_index+1;j<argc;j++) 
     {
       
                strcpy(producer_reg[i],argv[j]);
                ++i;
     }
     number_of_prod_files=i;
     printf("Files in the Producer are as below\n");
     for(j=0;j<number_of_prod_files;j++)
     {
               printf("%s\n",producer_reg[j]);
     }
 }
   int j=0;
   for(j=0;j<number_of_prod_files;j++)
   {
             for(i=0;i<number_of_cons_files;i++)
	     {
 		char checking[1000];
                memset(&checking,0,sizeof(checking));
		strcpy(checking,producer_reg[j]);
                strcat(checking,"/$");
                 if((strcmp(checking,consumer_reg[i]))==0)
                 {   
                    printf("Sincgle node cannot request the file it already has......Exiting the program\n");  
                    exit(0);
                 }
             }	 
   }
  
   pid_t pid=fork();
   if(pid==0)
   {
    Producer p;
    p.start();
   }
   else if(pid>0)
   {
    Consumer c;
    c.start_c();
   }

}
else
{
 printf("Wrong inputs entered..... Exiting");
 exit(0);
}
}
