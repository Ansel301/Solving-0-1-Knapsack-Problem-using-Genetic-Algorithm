#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<ctime>

#define C_RATE 0.6
#define M_RATE 0.01
#define RUN_TIME 30
using namespace std;

int main()
{
	int choice,eval_max,p_size;
	string input_file_path,output_file_path;
	cout<<"(1)dt01/item.txt"<<endl
		<<"(2)dt02/item.txt"<<endl
		<<"(3)dt03/item.txt"<<endl
		<<"(4)Choose other file"<<endl
		<<"Enter data file: ";
	cin>>choice;
	switch(choice)
	{
		case 1:
			input_file_path="dt01/item.txt";
			p_size=200;
			eval_max=1000/p_size;
			break;
		case 2:
			input_file_path="dt02/item.txt";
			p_size=1000;
			eval_max=10000/p_size;
			break;
		case 3:
			input_file_path="dt03/item.txt";
			p_size=100000;
			eval_max=500000/p_size;
			break;
		default:
			cout<<"Enter input file path: ";
			cin>>input_file_path;
			cout<<"Enter population size: ";
			cin>>p_size;
			cout<<"Enter evaluation max: ";
			cin>>eval_max;
			eval_max/=p_size;
	}
	
    ifstream data(input_file_path.c_str());
    if(!data.is_open())
    {
    	cout<<"Unable to open the file"<<endl;
    	return 0;
	}
	
	int items,capacity;
	float cp;
	vector<int> index;
	vector<float> cpv;
    data>>items>>capacity;
    int weight[items],value[items];
    for(int i=0;i<items;i++)
    {
    	data>>weight[i]>>value[i];
    	cp=(float)value[i]/(float)weight[i];
    	for(int j=0;j<cpv.size();j++)
    	{
    		if(cp>cpv[j])
    		{
    			cpv.insert(cpv.begin()+j,cp);
    			index.insert(index.begin()+j,i);
    			break;
			}
		}
		if(cpv.size()!=i+1)
		{
			cpv.push_back(cp);
    		index.push_back(i);
		}
	}
	data.close();
	
	srand(time(NULL));
	vector< vector<int> > population;
	vector<int> v,max;
	for(int run=1;run<=RUN_TIME;run++)
	{
		/*			generate p_size population			*/
		population.clear();
		for(int i=0;i<p_size;i++)
		{
			v.clear();
			for(int j=0;j<items;j++) v.push_back(rand()%2);
			population.push_back(v);
		}
		/*			generate p_size population			*/
		
		/*cout<<population.size()<<" "<<population[0].size()<<endl;
		for(int i=0;i<p_size;i++)
		{
			for(int j=0;j<items;j++) cout<<population[i][j]<<" ";
			cout<<endl;
		}*/
		
		/*			evaluation			*/
		int total_weight=0,total_value=0,idx;
		vector<int> values(p_size,0);
		for(int i=0;i<p_size;i++)
		{
			total_weight=0;
			total_value=0;
			for(int j=0;j<items;j++)
			{
				if(population[i][j])
				{
					total_weight+=weight[j];
					total_value+=value[j];
				}
			}
			idx=index.size()-1;
			while(total_weight>capacity)
			{
				if(population[i][index[idx]])
				{
					population[i][index[idx]]=0;
					total_weight-=weight[index[idx]];
					total_value-=value[index[idx]];
				}
				idx--;
			}
			values[i]=total_value;
		}
		/*			evaluation			*/
		
		int r1,r2;
		double rate,cp1,cp2;
		vector< vector<int> > v_temp;
		for(int iter=1;iter<eval_max;iter++)
		{
			v_temp.clear();
			/*			selection			*/
			for(int i=0;i<p_size;i++)
			{
				r1=rand()%p_size;
				do r2=rand()%p_size; while(r1==r2);
				if(values[r1]>values[r2]) v_temp.push_back(population[r1]);
				else v_temp.push_back(population[r2]);
			}
			/*			selection			*/
	
			/*			crossover			*/
			for(int i=0;i<p_size;i+=2)
			{
				rate=(double)rand()/RAND_MAX;
				if(rate<C_RATE)
				{
					for(int j=0;j<items;j++)
					{
						r1=rand()%2;
						if(r1) swap(v_temp[i][j],v_temp[i+1][j]);
					}
				}
			}
			/*			crossover			*/
	
			/*			mutation			*/
			for(int i=0;i<p_size;i++)
			{
				rate=(double)rand()/RAND_MAX;
				if(rate<M_RATE)
				{
					r1=rand()%items;
					v_temp[i][r1]=(v_temp[i][r1]+1)%2;
				}
			}
			/*			mutation			*/
			
			/*			evaluation			*/
			population.swap(v_temp);
			for(int i=0;i<p_size;i++)
			{
				total_weight=0;
				total_value=0;
				for(int j=0;j<items;j++)
				{
					if(population[i][j])
					{
						total_weight+=weight[j];
						total_value+=value[j];
					}
				}
				idx=index.size()-1;
				while(total_weight>capacity)
				{
					if(population[i][index[idx]])
					{
						population[i][index[idx]]=0;
						total_weight-=weight[index[idx]];
						total_value-=value[index[idx]];
					}
					idx--;
				}
				values[i]=total_value;
			}
			/*			evaluation			*/
		}
		
		//for(int i=0;i<items;i++) cout<<population[0][i];
		//cout<<endl;
		//for(int i=0;i<values.size();i++) cout<<values[i]<<endl;
		max.push_back(*max_element(values.begin(),values.end()));
		cout<<run<<". "<<max[run-1]<<endl;
	}
	
	int total=0;
	for(int i=0;i<max.size();i++) total+=max[i];
	cout<<endl<<"Average: "<<total/RUN_TIME<<endl;
    return 0;
}
