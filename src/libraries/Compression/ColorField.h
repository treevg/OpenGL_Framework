#ifndef COLORFIELD_H
#define COLORFIELD_H




class ColorField{
private:


public:
int appearence;
float color;
	ColorField();
	ColorField(int appearence, float color){
		this->appearence = appearence;
		this->color = color;
	}

};

#endif // COLORFIELD_H
