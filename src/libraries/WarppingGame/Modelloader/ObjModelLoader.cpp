#include <stdexcept>
#include <fstream>
#include <stdlib.h>

#include "ObjModelLoader.h"


//TODO -> change loadObj() to bool loadObj
//TODO ->  store particular meshes into map/vector
/* template< typename T >
T parseLine(const string& line, char* delim)  
{
          T t;
          vector<string> tokens;
         
          boost::split(tokens,line,boost::algorithm::is_any_of(delim));
          
          string val = tokens[0]; 
      
          if(val.compare("v")==0 || val.compare("vn")==0 ){
          
           float vert_array[3]; 
          
            for (int i = 1; i<4; i++){
               boost::algorithm::trim(tokens[i]);
               //cout << "debuggin loop " <<tokens[i]<< endl;
              float value =  boost::lexical_cast<float>(tokens[i]);
          //     cout<< "parsing string  " << value << endl;
              vert_array[i-1]=value;
              
          }

              t = {vert_array[0], vert_array[1], vert_array[2]};
        
          }else    
                  if(val.compare("vt")==0){

                float vert_array[2]; 
               for (int i = 1; i<3; i++){

                boost::algorithm::trim(tokens[i]);

          //    cout << "debuggin loop " <<tokens[i]<< endl;
            float value =  boost::lexical_cast<float>(tokens[i]);
          //      cout<< "parsing string  " << value << endl;

                vert_array[i-1]=value;         
          }

          t = {vert_array[0], vert_array[1]};
        
          } 
          
         return  t;
  
}
    

 
  ObjModelLoader::ObjModelLoader(const string& path){

   this->positions.clear();
   this->texturePositions.clear();
   this->normals.clear();
   this->faces.clear();

	
  this->loadMesh(path);
  //just for debuggin 
 //  printVectors();


 

	
}


void ObjModelLoader::loadMesh(const string& path) {
    
    vector<string> list; 

    ifstream reader; 

	reader.open(path, ios::in);
	
	if(!reader) {

     throw std::exception();

	}else {
   
    		while(!reader.eof()){
        string line; 
        getline(reader,line);		
       // cout<<line << endl;	
				this->processLines(line);

			}

     
	}



}
      void  ObjModelLoader::processLines(const string& line){

       string data_type=line.substr(0,2);

       if( data_type.compare("v ")==0) {
       Vertex v =  parseLine<Vertex>(line, " ");
       this->positions.push_back(v);
     //  cout <<"pos X: " << v.posX << endl;

       }
       if( data_type.compare("vn")==0) {
          Vertex normal = parseLine<Vertex>(line, " ");
          this->normals.push_back(normal);
        //  cout << "Pos Normal " << normal.posX << endl;
      }

      if( data_type.compare("vt")==0) {
          TextPos texture= parseLine<TextPos>(line, " ");
          this->texturePositions.push_back(texture);
        //  cout << "Pos texture " << texture.posU << endl;
      }

        if( data_type.compare("f ")==0) {
         
        parseFacesLine(line, "/ ");

      } 

    }

        
    void  ObjModelLoader::parseFacesLine(const string& line, char* delim){

          vector<string> tokens;
          boost::split (tokens,line,boost::algorithm::is_any_of(delim));

           int vert_array[3]; 
          
          for (int i = 1; i<tokens.size(); i++){
             boost::algorithm::trim(tokens[i]);

              if(tokens[i].empty())
              {
               vert_array[i%3]=-1;
                  
              }else{

               int value = lexical_cast<int>(tokens[i]);
                vert_array[i%3]=value; 

               if(i%3==0){
               /* cout<< " debugging " <<endl;
                cout<<" 0:  "<< vert_array[0];
                cout<<" 1:  " <<vert_array[1];
                cout<<" 2:  " << vert_array[2]; */

      /*            Face f = {vert_array[1], vert_array[2], vert_array[0]};
                  this->faces.push_back(f);

               }
               }
          }
           

         }
*/


    void ObjModelLoader::printVectors(){

        cout<< "------- debuging vectors -----"<<endl;

        cout<< "Vertecies: "<< endl;
        for (auto &v: this->positions) {
                cout<<v.posX   << " " <<v.posY << " " <<v.posZ<< endl;

        }

         cout<< "Normals: "<< endl;
        for (auto &n: this->normals) {
            cout<<n.posX   << " " <<n.posY << " " <<n.posZ<< endl;

        }

        cout<< "Textures: "<< endl;
        for (auto &t: this->texturePositions) {
            cout<<t.posU<< " " <<t.posV << endl;

        }

          cout<< "Faces: "<< endl;
        for (auto &f: this->faces) {
            cout<<f.vertexPos   << "  " <<f.texturePos << "  " << f.normalPos<< endl;
             
        }



    }


    vector<float> ObjModelLoader::getVerticies(){
     
            vector<float> verticies;
            verticies.clear();
             
              if(this->faces.size()!=0 && this->positions.size()!=0){
                
                for(int i=0; i<this->faces.size();++i){
                  
                  Vertex temp = this->positions[this->faces[i].vertexPos-1];
                 // cout << "Vector with verticies "<< temp.posX << " " <<temp.posY << " " << temp.posZ << endl; 

                  verticies.push_back(temp.posX);
                  verticies.push_back(temp.posY);
                  verticies.push_back(temp.posZ);

                }

            }   
               return verticies;


    }


    int ObjModelLoader::getNumberOfIndicies(){

        return this->faces.size(); 
    }


    vector<float> ObjModelLoader::getTexturePositions(){


           vector<float> uvPositions;
            uvPositions.clear();
             
              if(this->faces.size()!=0 && this->texturePositions.size()!=0){
                
                for(int i=0; i<this->faces.size();++i){
                  
                  TextPos temp = this->texturePositions[this->faces[i].texturePos-1];
             //     cout << "Texture positions "<< temp.posU << " " <<temp.posV << " "<< endl; 

                  uvPositions.push_back(temp.posU);
                  uvPositions.push_back(temp.posV);

                }

            }   
               return uvPositions;




    }