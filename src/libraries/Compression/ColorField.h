#ifndef COLORFIELD_H
#define COLORFIELD_H




class ColorField{
private:


public:
int appearence;
float r, g, b, a;
	ColorField();
	ColorField(int appearence, float rNew, float gNew, float bNew, float aNew){
		this->appearence = appearence;
		this->r = rNew;
		this->g = gNew;
		this->b = bNew;
		this->a = aNew;
	}

};

#endif // COLORFIELD_H
