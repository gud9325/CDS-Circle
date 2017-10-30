#include "ofApp.h"

std::string str;
std::vector<int> a2d;

void serial2int(int byteRead) {
	
	int myByte = byteRead;
	
	if (myByte == OF_SERIAL_NO_DATA) {
		cout << "no data was read" << endl;
	}
	else if (myByte == OF_SERIAL_ERROR) {
		cout << "en error occurred" << endl;
	}
	else if (myByte >= '0' && myByte <= '9') {
		str.push_back((char)myByte);
	}
	else if (myByte == 10 && !str.empty()) {
		int num = std::stoi(str);  
		str.clear();
		a2d.push_back(num);
	}
	else {
	}
}

float radiousScale(int num) {
	float inMin = 1012, inMax = 194;
	float outMin = 0, outMax = ofGetHeight() / 2;
	float rad = (num - inMin)*(outMax - outMin) / (inMax - inMin);
	return rad;
}

float pupilScale(int num) {
	float inMin = 194, inMax = 1012;
	float outMin = 0, outMax = ofGetHeight() * 40 / 319;
	float pupil = (num - inMin)*(outMax - outMin) / (inMax - inMin);
	return pupil;
}
//--------------------------------------------------------------
void ofApp::setup(){
	
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();

	int baud = 115200;
	mySerial.setup("COM3", baud);

	if (mySerial.isInitialized() == true) {
		cout << "serial Initialized" << endl;
		mySerial.flush();
	}
	else {
		cout << "serial not Initialized" << endl;
		return;
	}
	//ofBackground(54, 54, 54, 255); for Gray Background

	myImage.load("images/coloredEyeImage.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// The available method is useful when you want to know how many bytes are available in the serial port
	if (mySerial.available() > 0) {
		
		const int bufSize = 1024;
		unsigned char buf[bufSize];
		int bufReq = mySerial.available() > bufSize ? bufSize : mySerial.available();
		
		//Tries to read bytes from the connected serial device
		mySerial.readBytes(buf, bufReq);
		
		for (int i = 0; i < bufReq; i++) {
			serial2int(buf[i]);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255, 255, 255, 255);

	float imageHeight = ofGetWidth() * 319 / 500;
	float imageY = (ofGetHeight() - imageHeight) / 2;
	myImage.draw(0, imageY, ofGetWidth(), imageHeight);

	if (a2d.empty()) {
		printf("a2d empty\n");
		return;
	}
	else {
		int val = a2d.back();
		/* ofSetColor(255, 255, 0); for Yellow Circle
		float xCenter = ofGetWidth()/2;
		float yCenter = ofGetHeight()/2;
		
		ofDrawCircle(xCenter, yCenter, radiousScale(val)); for basic circle
		*/
		float xPupil = ofGetWidth() * 251 / 500;
		float yPupil = ofGetHeight() * 151 / 319;
		ofSetColor(0, 0, 0);
		ofDrawCircle(xPupil, yPupil, pupilScale(val));
	}
}
//--------------------------------------------------------------
