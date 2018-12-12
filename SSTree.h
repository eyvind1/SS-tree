#include <vector>
#include <iostream>
#include <math.h>
#include <list>
#include <algorithm>
typedef float Acceptor;
typedef float Visitor;
typedef float BoundedItem;

using namespace std; 
#define dimensions 91
class SSNode;
 
struct VisitorClass
{
	int count;
	bool ContinueVisiting;
	std::list<SSNode> result;

	VisitorClass() : count(0), ContinueVisiting(true) {};

	void operator()(const SSNode* leaf)
	{
	    result.push_back(leaf->leaf);
	}
};


class SSElem{
public:
	SSNode *child;
	vector<float> data;
	 SSElem(){
		 child=NULL;
		 }
	 void insertDate(float elems[]){
	 for(int i=0;i<dimensions;i++)
	 	{
			data.push_back(elems[i]);
		}

	 }

};

class SSNode{

public:
    int leaf;
	int total_children;
	int height;
	float radius;
	SSElem  *centroid;
	list <float> items;
	int  m_level;
	int m_count;
	float  varianza;
	int MAX;
	vector<SSElem*> Entry;
	SSNode(){
		Entry.resize(5);
		total_children=0;
	 	height=0;
	 	radius=0;
	 	m_level=0;
	 	 m_count=0;
	 	centroid=NULL;
	}
	SSNode(int maxE)
	{
     MAX=maxE;
     Entry.resize(MAX);
     total_children=0;
	 height=0;
	 radius=0;
	 m_level=0;
	 m_count=0;
	 centroid=NULL;
	}
	void cleaning(){
	     total_children=0;
		 height=0;
		 radius=0;
		 m_level=0;
		 m_count=0;
		 centroid=NULL;
		 for(int i=0;i<MAX;i++){
		 	Entry[i]=NULL;
		 }
	}
	bool isLeaf(){ 
		return (m_level == 0); 
		} 
};
class SSTree{
public:
	SSNode * root;
	int dimDate;
	int MAXDatos;
	SSTree()
	{
		root=NULL;
	}
    SSTree(int dimensionData,int maxNode){
    	dimDate=dimensionData;
    	MAXDatos=maxNode;
    	root=NULL;

    }
    void insert(float date[dimensions]);
    void searchRange(int date);
    Visitor query(const Acceptor &accept, Visitor visitor);


    private:
     vector<SSNode*> camino;

    void chooseLeaf(SSElem *elemento,SSNode*& p);
    SSElem* calcularCentroide(SSNode*p);
    float distanciaEucleadana(SSElem *elementoU,SSElem *elementoD);
    float calcularRadio(SSNode*p);
    void split(vector<SSElem*> Entry,SSElem *centro,SSNode *padre);
};

void SSTree::chooseLeaf(SSElem *elemento,SSNode*& p){       
		 p=root;
        float auxArea, minArea, tmpArea, tmpEnlargement,enlargement;
        camino.push_back(root);
      
        while ((p)and (not p->isLeaf())){
         
                for(int i=1;i<p->m_count;i++)
                {
                    tmpEnlargement=p->m_entry[i]->m_mbr->calEnlargement(*mbr);
                    if(tmpEnlargement < enlargement) {
                            enlargement = tmpEnlargement;
                         index = i;
                    }
                    else if(tmpEnlargement == enlargement){
                            enlargement = tmpEnlargement;
                            index = i;
                    }

                 }
                p=p->m_entry[index]->m_child;
                indices.push_back(index);
                camino.push_back(p);	

        }
}
SSElem* SSTree::calcularCentroide(SSNode*p){
	float  centro[dimensions];
	for(int i=0;i<dimensions;i++){centro[i]=0;}
	SSElem *tmp;

	for(int i=0;i<p->m_count;i++) 
	{
        for(int j=0;j<dimensions;j++)
        {
        	centro[j]=(centro[j]+p->Entry[i]->data[j])/2;
        }
	} 
	tmp=new SSElem();
	tmp->insertDate(centro);
    return tmp;
}
float SSTree::distanciaEucleadana(SSElem *elementoU,SSElem *elementoD)
{  float suma=0;
	for(int i=0;i<dimensions;i++)
	{

      suma=suma + (elementoD->data[i]-elementoU->data[i]);
	}
	return sqrt(suma);

}
 float SSTree::calcularRadio(SSNode*p){
	float radio=0,distancia=0;
       for(int j=0;j<p->m_count;j++)
       {
          distancia=distanciaEucleadana(p->centroid,p->Entry[j]);
          if(distancia>radio){
          	 radio=distancia;
          	}
        }
    return radio;
}
void SSTree::insert(float date[dimensions]){
 		SSNode *p;
 		SSElem *elemento=new SSElem();
 		elemento->insertDate(date);
 		chooseLeaf(elemento,p);

 		if(!p){
 			root=new SSNode(MAXDatos);
 			root->Entry[root->m_count]=elemento;
 			root->m_count+=1;
 			root->centroid=elemento;
 			root->Radius=0;
 			root->Variance=0;
 			root->height=1;
 			root->total_children=1;
 		}
 		else if(p->m_count<MAXDatos)
 		{
 			p->Entry[p->m_count]=elemento;
 			p->m_count+=1;
 			p->centroid=calcularCentroide(p);
 			p->Radius=calcularRadio(p);
 		}
 		else
 		{
           p->m_level+=1;
           p->m_count+=1;

           split(p->Entry,elemento,p);
           
   

 		}
}
void SSTree::split(vector<SSElem*> Entry,SSElem *centro,SSNode *padre){
      vector<SSElem*> entradas;
      SSElem *tmp1,*tmp2;
      int distancia,distanciaActual,indexX,indexY;

      for(int i=0;i<Entry.size();i++){ entradas.push_back(Entry[i]);}
      entradas.push_back(centro);
      for(int i=0;i<entradas.size();i++)
      {
      	for(int j=i+1;j<entradas.size();j++)
      	{
         distancia=distanciaEucleadana(entradas[i],entradas[j]);
         if(distancia>distanciaActual)
         	distanciaActual=distancia;
            tmp1=entradas[i];
            tmp2=entradas[j];
            indexX=i;
            indexY=j;

      	}
      }
      entradas.erase( entradas.begin()+ indexX); 
      entradas.erase( entradas.begin()+ indexY);
      SSNode *aux,*aux1;
      aux=new SSNode(MAXDatos);
      aux->Entry[aux->m_count]=tmp1;
      aux->m_count+=1;
      aux1=new SSNode(MAXDatos);
      aux->Entry[aux->m_count]=tmp2;
      aux->m_count+=1;

       for(int i=0;i<entradas.size();i++)
      {   distancia=distanciaEucleadana(tmp1,entradas[i]);
	      if(distancia<=(distanciaActual/2)){
	         	aux->Entry[aux->m_count]=entradas[i];
	         	aux->m_count+=1;
	            entradas.erase( entradas.begin()+ i); }

      }
      for(int i=0;i<entradas.size();i++)
      {   distancia=distanciaEucleadana(tmp2,entradas[i]);
	      if(distancia<(distanciaActual/2)){
	         	aux1->Entry[aux1->m_count]=entradas[i];
	         	aux1->m_count+=1; 
	            entradas.erase( entradas.begin()+ i);
	            }
      }
     aux->m_level=aux1->m_level=(padre->m_level)+1;
     aux->height=aux1->height=(padre->height)+1;
     aux->centroid=calcularCentroide(aux);
 	 aux->Radius=calcularRadio(aux);
 	 aux1->centroid=calcularCentroide(aux1);
 	 aux1->Radius=calcularRadio(aux1);

 	 padre->cleaning();
 	 padre->Entry[padre->m_count]->child=aux;
 	 padre->m_count+=1;
 	 padre->Entry[padre->m_count]->child=aux1;
 	 padre->m_count+=1;
 	}



	Visitor SSTree::query(const Acceptor &accept, Visitor visitor)
	{
		if (root)
		{	
			QueryFunctor query(accept, visitor);
			query(root);
		}
		
		return visitor;
	}

	
	struct QueryFunctor   {
		const Acceptor &accept;
		Visitor &visitor;
		
		explicit QueryFunctor(const Acceptor &a, Visitor &v) : accept(a), visitor(v) {}
	
		void operator()(SSNode * item)
		{
			SSNode * node = item;
		
			if (node)
			{
				if (node->leaf)
					for_each(node->items.begin(), node->items.end(), VisitFunctor(accept, visitor));
				else
					for_each(node->items.begin(), node->items.end(), *this);
			}
		}
	};
	struct VisitFunctor {
	
		const Acceptor &accept;
		Visitor &visit;
		
		explicit VisitFunctor(const Acceptor &a, Visitor &v) : accept(a), visit(v) {}
	
		SSNode* operator()( SSNode * item ) 
		{
			SSNode * node = (item);
		
			if (node)
				return node;
			else
				item->leaf = node->leaf;
		}
	};