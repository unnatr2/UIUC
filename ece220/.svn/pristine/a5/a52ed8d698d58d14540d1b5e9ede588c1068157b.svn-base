// ECE220 MP12
// Developer: Unnat Ramjiyani
// NetID: unnatr2

// The following code reads data from a file and creates shape objects using the
// base class Shape and the derived classes Rectangle, Circle, Sphere and RectPrism.
// The objects are saves in a vector of shapes and is used to find the maximum
// area and maximum volume of all the shapes. The shapes have added functionality
// where the user can add or subtract two shape objects to get a new shape object.

#include "shape.hpp"



// Base Class: Shape
Shape::Shape(string name){	// Constructor
	name_=name;
}

// Function: getName()
// Returns the name of the shape
string Shape::getName(){
	return name_;
}



// Derived Class: Rectangle
Rectangle::Rectangle(double width, double length):Shape("Rectangle"){	// Constructor
	width_=width;
	length_=length;
}

// Function: getWidth()
// Returns width of rectangle
double Rectangle::getWidth()const{return width_;}

// Function: getLength()
// Returns length of rectangle
double Rectangle::getLength()const{return length_;}

// Function: getArea()
// Retruns area of rectangle
double Rectangle::getArea()const{
	return (width_*length_);
}

// Function: getVolume()
// Returns volume of rectangle
double Rectangle::getVolume()const{
	return 0;
}

// Operator: +
// Returns rectangle with the additive lengths and widths of two other rectangles
Rectangle Rectangle::operator +(const Rectangle& rec){
	double length_new=length_+rec.length_;
	double width_new=width_+rec.width_;
	Rectangle newRec(width_new,length_new);
	return newRec;
}

// Operator: -
// Returns rectangle with the difference of lengths and widths of two other rectangles
Rectangle Rectangle::operator -(const Rectangle& rec){
	double length_new=max(0.0,length_-rec.length_);
	double width_new=max(0.0,width_-rec.width_);
	Rectangle newRec(width_new,length_new);
	return newRec;
}



// Derived Class: Circle
Circle::Circle(double radius):Shape("Circle"){ // Constructor
	radius_=radius;
}

// Function: getRadius()
// Returns the radius of circle
double Circle::getRadius()const{return radius_;}

// Function: getArea()
// Returns the area of circle
double Circle::getArea() const{
	return M_PI*radius_*radius_;
}

// Function: getVolume()
// Returns the volume of circle
double Circle::getVolume() const{
	return 0;
}

// Operator: +
// Returns circle with additive radii of two other circles
Circle Circle::operator + (const Circle& cir){
	double radius_new=radius_+cir.radius_;
	Circle newCir(radius_new);
	return newCir;
}

// Operator: -
// Returns circle with difference of radii of two other circles
Circle Circle::operator - (const Circle& cir){
	double radius_new=max(0.0,radius_-cir.radius_);
	Circle newCir(radius_new);
	return newCir;
}



// Derived Class: Sphere
Sphere::Sphere(double radius):Shape("Sphere"){ // Constructor
	radius_=radius;
}

// Function: getVolume()
// Returns the volume of sphere
double Sphere::getVolume() const{
	return (4.0/3.0)*M_PI*radius_*radius_*radius_;
}

// Function: getArea()
// Returns the area of sphere
double Sphere::getArea() const{
	return 4*M_PI*radius_*radius_;
}

// Operator: +
// Returns sphere with additive radii of two other spheres
Sphere Sphere::operator + (const Sphere& sph){
	double radius_new=radius_+sph.radius_;
	Sphere newSph(radius_new);
	return newSph;
}

// Operator: -
// Returns sphere with difference of radii of two other spheres
Sphere Sphere::operator - (const Sphere& sph){
	double radius_new=max(0.0,radius_-sph.radius_);
	Sphere newSph(radius_new);
	return newSph;
}

// Function: getRadius()
// Returns the radius of sphere
double Sphere::getRadius()const{return radius_;}



// Derived Class: RectPrism
RectPrism::RectPrism(double width, double length, double height):Shape("RectPrism"){ // Constructor
	width_=width;
	length_=length;
	height_=height;
}

// Function: getVolume()
// Returns the volume of rectangular prism
double RectPrism::getVolume() const{
	return width_*length_*height_;
}

// Function: getArea()
// Returns the area of rectangular prism
double RectPrism::getArea() const{
	return 2*((width_*length_)+(length_*height_)+(height_*width_));
}

// Operator: +
// Returns rectangular prism with additive lengths, widths and heights of two other rectangular prisms
RectPrism RectPrism::operator + (const RectPrism& rectp){
	double length_new=length_+rectp.length_;
	double width_new=width_+rectp.width_;
	double height_new=height_+rectp.height_;
	RectPrism newRectp(width_new,length_new,height_new);
	return newRectp;
}

// Operator: -
// Returns rectangular prism with difference of lengths, widths and heights of two other rectangular prisms
RectPrism RectPrism::operator - (const RectPrism& rectp){
	double length_new=max(0.0,length_-rectp.length_);
	double width_new=max(0.0,width_-rectp.width_);
	double height_new=max(0.0,height_-rectp.height_);
	RectPrism newRectp(width_new,length_new,height_new);
	return newRectp;
}

// Function: getWidth()
// Returns the width of rectangular prism
double RectPrism::getWidth()const{return width_;}

// Function: getHeight()
// Returns the height of rectangular prism
double RectPrism::getHeight()const{return height_;}

// Function: getLength()
// Returns the length of rectangular prism
double RectPrism::getLength()const{return length_;}



// Function: CreateShapes()
// Reads data from file, creates shape objects and pushs them onto a vector of shapes
// Returns vector
vector<Shape*> CreateShapes(char* file_name){
	vector<Shape*> vect; // Declare vector
	Shape* temp; // Declare temp shape object
	string name; // Declare variable
	double x,y,z; // Declare variables
	int size; // Declare variable
	ifstream file(file_name,std::ifstream::in); // Open file
	file >> size; // Read size
	for (int i=0; i<size; i++){ // Loop through shapes
		file >> name; // Get name of shape
		if (name=="Rectangle"){ // If rectangle
			file >> x >> y; // Get dimentions
			temp=new Rectangle(x,y); // Create new rectangle
			vect.push_back(temp); // Push onto vector
		}
		else if (name=="Circle"){ // If circle
			file >> x; // Get dimentions
			temp=new Circle(x); // Create new circle
			vect.push_back(temp); // Push onto vector
		}
		else if (name=="Sphere"){ // If sphere
			file >> x; // Get dimentions
			temp=new Sphere(x); // Create new sphere
			vect.push_back(temp); // Push onto vector
		}
		else { // If rectangular prism
			file >> x >> y >> z; // Get dimentions
			temp=new RectPrism(x,y,z); // Create new rectangular prism
			vect.push_back(temp); // Push onto vector
		}
	}
	file.close(); // Close file
	return vect; // Return  vector
}



// Function: MaxArea()
// Call getArea() of each object
// Return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
	int size=shapes.size();
	for (int i=0;i<size;i++){
		if (max_area<shapes[i]->getArea()){
			max_area=shapes[i]->getArea();
		}
	}
	return max_area;
}



// Function: MaxVolume()
// Call getVolume() of each object
// Return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	int size=shapes.size();
	for (int i=0;i<size;i++){
		if (max_volume<shapes[i]->getVolume()){
			max_volume=shapes[i]->getVolume();
		}
	}
	return max_volume;
}
