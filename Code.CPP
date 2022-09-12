#include<iostream.h>
#include<string.h>
#include<conio.h>
#include<fstream.h>
#include<process.h>
#include<stdlib.h>
#include<math.h>
#include<values.h>
#include<stdio.h>
fstream fp;
int recount=0;
char fname[20]="itemstore.txt";
struct item
{
char mitemid[15];
char mname[15];
float price,qty,tax,dis;
};
item buffer;
void modify();
void billing();
void unpack(char[]);
void display();
void insrec();
void delrec();
void search_by_name();
int search(char[]);
struct primaryindex
{
char pkey[15];
int offset;
};
struct secondaryindex
{
   char skey[15];
	char pkey[15];
};

primaryindex pindex[100];
secondaryindex sindex[100];
void unpack(char buffer[])  
{
char *t;
t=strtok(buffer,"|");
cout<<"itemid:"<<t<<"\t\t";
t=strtok(NULL,"|");
cout<<"name:"<<t<<"\t\t";
t=strtok(NULL,"|");
cout<<"price:"<<t<<endl;
t=strtok(NULL,"|");
cout<<"quantity:"<<t<<"\t\t";
t=strtok(NULL,"|");
cout<<"Tax:"<<t<<"\t\t\t";
t=strtok(NULL,'\0');
cout<<"discount:"<<t<<endl;
}
void insrec()
{
item buffer,tem;
char *itemid;
int i,j,pos;
cout<<"enter the item id,name.price.quantity,tax and discount\n";
cout<<"item id:";
cin>>buffer.mitemid;
cout<<"item name:";
cin>>buffer.mname;
cout<<"price:";
cin>>buffer.price;
cout<<"quantity:";
cin>>buffer.qty;
cout<<"tax:";
cin>>buffer.tax;
cout<<"discount:";
cin>>buffer.dis,
pos=search(buffer.mitemid);
if(pos!=0)
{
cout<<"record already present";
fp.close();
return;
}
recount++;
fp.open(fname,ios::out | ios::ate);
strcpy(pindex[recount].pkey,buffer.mitemid);
pindex[recount].offset=fp.tellg();
fp<<buffer.mitemid<<"|"<<buffer.mname<<"|"<<buffer.price<<"|"<<buffer.qty<<"|"<<buffer.tax<<"|"<<buffer.dis;
fp<<"\n";
primaryindex temp;
for(i=1;i<recount;i++)
{
for(j=1;j<recount;j++)
{
if(strcmp(pindex[j].pkey,pindex[j+1].pkey)>0)
{
strcpy(temp.pkey,pindex[j].pkey);
temp.offset=pindex[j].offset;
strcpy(pindex[j].pkey,pindex[j+1].pkey);
pindex[j].offset=pindex[j+1].offset;
strcpy(pindex[j+1].pkey,temp.pkey);
pindex[j+1].offset=temp.offset; 
}
}
}
fp.close();
}
void delrec()
{
char mitemid[15];
int pos,i,k;
cout<<"\n enter the item id to be deleted: \n";
cin>>mitemid;
fp.open(fname,ios::out | ios::ate);
pos=search(mitemid);
if(pos==0)
{
cout<<"\n record not found \n";
fp.close();
return;
}
fp.seekg(pindex[pos].offset,ios::beg);
fp<<"*";
fp.close();
for(i=pos;i<recount;i++)
{
strcpy(pindex[i].pkey,pindex[i+1].pkey);
pindex[i].offset=pindex[i+1].offset;
}
cout<<"record deleted successfully"<<endl;
recount--;
}
int search(char mitemid[])
{
int low=1,high=recount,mid;
while(low<=high) 
{
mid=(low+high)/2;
if(strcmp(pindex[mid].pkey,mitemid)==0)
return mid;
if(strcmp(pindex[mid].pkey,mitemid)>0)
high=mid-1;
else
low=mid+1;
}
return 0;
}
void display()
{
char buffer[100];
int i,j;
char str[50];
fp.open(fname,ios::in);
if(recount==0)
{
cout<<"no records to display"<<endl;
fp.close();
return;
}
cout<<"...file contents are ... "<<endl;
for(i=1;i<=recount;i++)
{
cout<<"\n"; 
fp.seekg(pindex[i].offset,ios::beg);
fp>>str;
unpack(str);
}
fp.close();
}
void search_by_name()
{
   char record[50];
char mname[15];
int i,j;
int pos;
fp.open(fname,ios::in);
cout<<"Enter the secondary key(name) \n";
cin>>mname;
for(i=1;i<=recount;i++)
{
if(strcmp(sindex[i].skey,mname)<0)
{
pos=search(sindex[i].pkey);
fp.seekg(pindex[pos].offset,ios::beg);
fp>>record;
unpack(record);
}
else
{
cout<<"Record not found";
}
}
fp.close();
}

void modify()
{
char *mitemid,*no,*mname,*price,*qty,*tax,*dis;
char itemid[15],new_price[15],new_qty[15];
char record[100];
int i,j,pos,flag=0;
cout<<"enter the item id to be modified \n";
cin>>itemid;
fp.open(fname,ios::in | ios::out);
for(i=1;i<=recount;i++)
{
if(strcmp(pindex[i].pkey,itemid)==0) 
{
flag=1;
pos=search(pindex[i].pkey);
break; 
}
}
if(flag==0)
{
cout<<"item not found\n";
}
else
{
fp.seekg(pindex[pos].offset,ios::beg);
fp>>record;
cout<<"....... record before modification......... \n";
mitemid=strtok(record,"|");
mname=strtok(NULL,"|");
price=strtok(NULL,"|");
qty=strtok(NULL,"|");
tax=strtok(NULL,"|");
dis=strtok(NULL,'\0');
cout<<"item id:"<<mitemid<<endl;
cout<<"item name:"<<mname<<endl;
cout<<"price:"<<price<<endl;
cout<<"quantity:"<<qty<<endl;
cout<<"tax:"<<tax<<endl;
cout<<"discount:"<<dis<<endl;
cout<<"enter the new rate and quantity\n";
cin>>new_price;
cin>>new_qty;
fp.seekp(pindex[pos].offset,ios::beg);
fp<<no<<"|"<<mitemid<<"|"<<mname<<"|"<<new_price<<"|"<<new_qty<<"|"<<tax<<"|"<<dis;
fp<<"\n";
cout<<" ........modified record is......... \n"; 
fp.seekg(pindex[pos].offset,ios::beg);
fp>>record;
unpack(record);
}
fp.close();
}
void billing()
{
char itemids[15][15],buffer[100],ratel;
char *itemid,*name,*price,*qty,*tax,*dis;
int i,n,pos;
float gross,cost,totalcost=0.0;
int qtys[15];
cout<<"enter no. of items purchased by the customer\n";
cin>>n;
cout<<"select the item id and their quantities purchased by the customer\n";
for(i=1;i<=n;i++) 
{
cout<<"item id:";
cin>>itemids[i];
cout<<endl;
cout<<"item quantity:";
cin>>qtys[i];
cout<<endl; 
}
fp.open(fname,ios::in | ios::out);
for(i=1;i<=n;i++)
{
pos=search(itemids[i]);
fp.seekg(pindex[pos].offset,ios::beg);
fp>>buffer;
itemid=strtok(buffer,"|");
name=strtok(NULL,"|");
price=strtok(NULL,"|");
qty=strtok(NULL,"|");
tax=strtok(NULL,"|"); 
dis=strtok(NULL,'\0');
int x=atof(price);
int y=atof(tax);
int dis1=atof(dis);
gross=x+(x*(y/100));
cost=qtys[i]*(gross-(gross*(dis1/100)));
float avail1=atof(qty);
if(avail1==0.0)
{
cout<<itemid<<" is out of stock\n";
continue;
}
float new_avail=avail1-qtys[i];
fp.seekp(pindex[pos].offset,ios::beg);
fp<<itemid<<"|"<<name<<"|"<<price<<"|"<<new_avail<<"|"<<tax<<"|"<<dis; 
totalcost=totalcost+cost;
}
cout<<"total cost to be paid="<<totalcost<<endl;
fp.close();
}
void main()
{
char buffer[100];
item ml;
int i,j,choice,k,pos;
char mno[15],mitemid[15];
clrscr();
cout<<"\t...SUPERMARKET BILLING SYSTEM...\t"<<endl;
fp.open(fname,ios::out);
fp.close();
for(;;)
{
cout<<" l.insert item  2.delete item  3.display all item  4.search by item id  5.modify the rate and quantity of item 6 billing 7 search using name 8 exit ";
cout<<"\n enter your choice:\n";
cin>>choice;
switch(choice)
{
case 1:insrec();
break;
case 2:delrec();
break;
case 3:display();
break;
case 4:cout<<"\nenter the item id to be searched: \n";
cin>>mitemid;
pos=search(mitemid);
if(pos==0)
{
cout<<"item id not found\n";
break;
}
fp.open(fname,ios::in);
fp.seekg(pindex[pos].offset,ios::beg);
fp>>buffer;
unpack(buffer);
fp.close();
break;
case 5:modify();
break;
case 6:billing();
break;
case 7:search_by_name();
break;
case 8:exit(0);
}
}
}