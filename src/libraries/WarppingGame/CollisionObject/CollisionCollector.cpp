#include "CollisionCollection.h"

using namespace std;

 CollisionCollection::CollisionCollection():collisionObjects(0) {}

 CollisionCollection::~CollisionCollection(){

	for (auto co : this->collisionObjects){
        delete co;
    }
 }
 
 //mesh to Collision Object and push into array

    void CollisionCollection::add(vector<VertexArrayObject*> meshes, mat4 modelmatrix, string k){

    		for(VertexArrayObject* mesh: meshes){

    			this->add(mesh, modelmatrix, k);

    		}



    }
    //convert mesh to Collision object and push down into vector
    
    void CollisionCollection::add(VertexArrayObject* mesh, mat4 modelmatrix, string k){

      int id = this->collisionObjects.size();
      cout << "id is " << id << endl;
      CollisionObject* co= new CollisionObject( mesh, modelmatrix, id, k);
      this->add(co);
    
    }

    
    void CollisionCollection::add(CollisionObject* co){
    	
    	this->collisionObjects.push_back(co);

    }

    bool CollisionCollection::checkCollision(Player* player){

    	bool collision = false;

    	for(CollisionObject* co: this->collisionObjects){
    		collision = co->collidesWothOtherWithPlayer(player);
    	
        	if(collision == true ){
                cout << "collision with key : " << co->getKey() << " id:  " << co->getId() << endl;
    			break;
    		}
    	}

    	return collision;
    }
