// Z tych  klas u¿ywamy QueueHist i metod: put() ,get(), printPv()

#pragma once 
#include<vector>
#include<iostream>
#include<fstream>
#include<assert.h>
#include <omnetpp.h>
#include<math.h>
using namespace std;
using namespace omnetpp;
class Histogram 
  {
      simtime_t simtime;
    protected:
      vector<double>  *hist;
      int size;
    public:
      Histogram(int size_)
        {
          size=size_;
          simtime=0;
          hist=new vector<double>(size);
          for(int i=0;i<size;i++) 
               (*hist)[i]=0;
        }
     void  set(simtime_t val,int i)
        {
          if(i<size)
           (*hist)[i]+=val.dbl();
          else
           (*hist)[size-1]+=val.dbl();
          simtime+=val;
        }
     double createPv()
	 // wywolujemy pod koniec symulacji - buduje histogram i zwraca sredni¹ d³ugoœæ kolejki
        {
          double mean=0;
          for(int i=0;i<size;i++)
            (*hist)[i]=(*hist)[i]/simtime.dbl();  
          for(int i=0;i<size;i++)
            mean+=((*hist)[i])*i;
          return mean;
        }
      void printPv(ostream &str=cout) // drukuje histogram
          {
            for(int i=0;i<size;i++)
              {
                if((*hist)[i]>0)
                  str<<i<<" "<<(*hist)[i]<<endl;
              }
          }
      
  };
 
class QueueHist: public Histogram //klasa buduje historam na podstawie próbek czasowych
  {
    
      simtime_t told;
      int nb_of_clients;
    public:  
      QueueHist(int size=10000):Histogram(size) 
	  // size  = maksymalna liczba paietow w kolejce + 1 (kolejka pusta)
        {
          told=0;
          nb_of_clients=0;
        };
      void put() // wywolujemy przy kazdym wsawianiu do kolejki
        {
          set(simTime()-told,nb_of_clients);
          told=simTime();
          nb_of_clients++;
        };
      void get() // wywolujemy przy usuwaniu pakietu z  kolejki
        {
          set(simTime()-told,nb_of_clients);
          told=simTime();
          nb_of_clients--;
        };
     
  };
   
   
 class DoubleHist
  {  
    vector<unsigned long long>  hist;
    int res;
    int size;
     
    void norm()// res=resolution 
        {
          for(int i=0;i<size;i++)
            sum+=hist[i];
       
        };
   public:
      unsigned long long int sum;
      DoubleHist(int range,int res_):hist((range+1)*res_)// res=resolution 
        {
          res=res_;
          sum=0;
          size=(range+1)*res;
          for(int i=0;i<size;i++)
            hist[i]=0;
        };
      void put(double val)
        { 
          int i=(int)round(val*res);
         // cout<<val*res<<" "<<i<<" "<<hist[i]<<endl;
          if(i<size)
           hist[i]+=1;
          else
            assert(false);
        };
     void printPv(ostream &str=cout)//; &str=cout)
        { 
          //norm();
          double x=0;
          norm();
          //str<<"******hh***********\n";
          for(int i=0;i<size;i++)
            {
              if(hist[i]>0)
                {
                  str<<x<<" "<<(double)hist[i]/sum<<endl;
                  x+=1.0/(double)res;
                }
            }
        }
         
  };
