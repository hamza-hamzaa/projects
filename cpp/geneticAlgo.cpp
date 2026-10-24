/*
this was my first introduction to machine learnign and in this time i was still using c++ for my ML projects
it was a genetic algorithm that taught me breeding, natural selection ,mutations elitism and 
tournament style selections and a fitness function which acts similar to a cost function 
*/

#include<iostream>
#include<cstdlib>
#include<random>
#include<algorithm>
#include<ctime>
#include<map>
#include<matplot/matplot.h>
using namespace std;

const string answer="hamza ";
const string chars ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890/,. ";
struct solution{
    string ans;
    float rank;
    solution(int size,char c):ans(size,c),rank(0){}
    solution(string a):ans(a),rank(0){}
};


class solutions{
    private:
        vector<solution> population;
        int size=answer.length();
        int populationSize=1000;
        bool found;

        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> dist;
        int generation;
        map<int,int> bestFitPerGen;
        int count=0;
    
    

        vector<solution> privBreed(int c){
            vector<solution> newPopulation;
            for(int i=0;i<populationSize/2-c/2;i++){
                int pos1= rand()%populationSize/2;
                int pos2= rand()%populationSize/2;
                int sep=(rand()%(size-1))+1;
                string str1=population[pos1].ans.substr(0,sep)+population[pos2].ans.substr(sep,size);
                string str2=population[pos2].ans.substr(0,sep)+population[pos1].ans.substr(sep,size);
                newPopulation.push_back(mutate(solution(str1)));
                newPopulation.push_back(mutate(solution(str2)));
                count+=2;
            }
            return newPopulation;
        }
        void tournamentSelection(){
            vector<solution> newPopulation;
            vector<solution> tournamentA;
            vector<solution> tournamentB;
            int pos1;
            for(int i=0;i<populationSize/2;i++){
                tournamentA.clear();
                tournamentB.clear();
                for(int i=0;i<4;i++){
                    pos1= rand()%populationSize/2;
                    tournamentA.push_back(population[pos1]);
                }
                for(int i=0;i<4;i++){
                    pos1= rand()%populationSize/2;
                    tournamentB.push_back(population[pos1]);
                }
                sort(tournamentA.begin(), tournamentA.end(),
                [](const solution& a, const solution& b) {
                    return a.rank > b.rank;});

                sort(tournamentB.begin(), tournamentB.end(),
                [](const solution& a, const solution& b) {
                    return a.rank > b.rank;});

                int sep=(rand()%(size-1))+1;
                string str1=tournamentA[0].ans.substr(0,sep)+tournamentB[0].ans.substr(sep,size);
                string str2=tournamentB[0].ans.substr(0,sep)+tournamentA[0].ans.substr(sep,size);
                newPopulation.push_back(mutate(solution(str1)));
                newPopulation.push_back(mutate(solution(str2)));
                count+=2;
            }
            population=newPopulation;
        }
        void elitism(int c){
            vector<solution>newPopulation;
            vector<solution> breeded=privBreed(c);
            for(int i =0;i<c;i++){
                newPopulation.push_back(population[i]);
                count++;
            }
            newPopulation.insert(newPopulation.end(),breeded.begin(),breeded.end());
            population=newPopulation;
        }

    public:
        solutions():gen(rd()),dist(0,chars.length()-1),found(false),generation(1){
            
            solution n(size,' ');
            for(int i=0;i<populationSize;i++){
                for(int j=0;j<size;j++){
                    n.ans[j]= chars[dist(gen)];
                }
                count++;
                population.push_back(n);
            }



        }
        void fitness(){
            for(int i=0;i<population.size();i++){
                population[i].rank=0;
                for(int j=0;j<size;j++){
                    if(population[i].ans[j]==answer[j]){
                        population[i].rank++;
                    }
                }
            }
            sort(population.begin(), population.end(),
            [](const solution& a, const solution& b) {
                return a.rank > b.rank;   // highest rank first
            });
        }
        solution mutate(solution current){
            int div =current.rank;
            if(current.rank==0||current.rank==1){div=2;}
            if(current.rank==11){return current;}
            solution mutated=current;
            int chance=rand()%div;
            int pos = rand()%size;
            if(chance==1){
                mutated.ans[pos]=chars[dist(gen)];
            }
            return mutated;
        }
        void breed(){
            this->elitism(6);
            this->fitness();
            bestFitPerGen[generation]=population[0].rank;
            generation++;


        }

        void print(){
            for(int i=0;i<population.size();i++){
                if(population[i].ans==answer){
                    cout<<population[i].ans<<" "<<population[i].rank<<endl;
                    found=true;
                    return;
                }
                else{
                    cout<<population[i].ans<<" "<<population[i].rank<<endl;
                }
                
            }    
        }
        bool getFound(){return found;}
        int getCount(){return count;}
        int getGeneration(){return generation;}
        void printMap(){
            for(auto& d:bestFitPerGen){
                cout<<d.first<<" "<<d.second<<endl;
            }
        }
        void graph(){
            vector<int>gen;
            vector<int> maxRank;
            for(auto& d:bestFitPerGen){
                gen.push_back(d.first);
                maxRank.push_back(d.second);
            }
            matplot::plot(gen, maxRank, "--gs")->line_width(2).marker_size(10).marker_color("b").marker_face_color({.5, .5, .5});
            matplot::show();
        }


};
int main(){
    srand(time(0));
    
    solutions* test= new solutions();
    test->fitness();
    test->print();
    while(!test->getFound()){
        test->breed();
        test->print();
    }
    test->printMap();
    cout<<test->getCount()<<endl;
    cout<<test->getGeneration();

   
   
    
    
}










