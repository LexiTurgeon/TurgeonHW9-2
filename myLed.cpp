#include <stdlib.h>
#include <sys/sysinfo.h>
#include <cgicc/Cgicc.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <unistd.h>
#include <iostream>
#include <string>
//#include "GPIO.h"

const std::string LED_GPIO = "/sys/class/gpio/gpio60/";
using namespace cgicc;
using namespace std;

void writeGPIO(std::string filename, std::string value){
	std::fstream fs;
	fs.open((LED_GPIO + filename).c_str(), std::fstream::out);
	fs << value;
	fs.close();
}

int main(int argc, char* argv[]) {
	string cmd;
	string number = "-1";
	Cgicc form;
	//GPIO inPin(60);
	writeGPIO("direction", "out");
	bool isStatus = form.queryCheckbox("status");
	form_iterator it = form.getElement("command");
	
	if (it == form.getElements().end() || it->getValue() == ""){
		cmd = "off"; // Output off
	}
	else {
		cmd = it->getValue();
	}

	form_iterator i = form.getElement("number");

	if (i == form.getElements().end() || it->getValue() == "") {
		number = "0";
	}
	else {
		number = i->getValue();
	}

	char *value = getenv("REMOTE_ADDR");
	cout << HTTPHTMLHeader() << endl;
	cout << html() << head() << title("CPE 422/522 Project: Post LED Example") << head() << endl;
	cout << "<h1\"> CPE 422/522 Project: Post LED Example </h1>" << h1("Controlling a GPIO pin via a web browser") << endl;
	cout << "<form action=\"/cgi-bin/myLed.cgi\" method=\"POST\">\n";
	cout << "<div>Set LED: <input type=\"radio\" name=\"command\" value=\"on\"" << ( cmd=="on" ? "checked":"") << "/> On ";
	cout << "<input type=\"radio\" name=\"command\" value=\"off\"" << ( cmd=="off" ? "checked":"") << "/> Off ";
	cout << "<input type=\"radio\" name=\"command\" value=\"blink\"" << ( cmd=="blink" ? "checked":"") << "/> Blink";
	cout << "<input type=\"submit\" style=\"background-color:green;\" value=\"Set LED\" />";
	cout << "</div> <br>";
	cout << "<div>number of Blinks: <input type=\"text\" name=\"number\" />";
	cout << "</div></form></body>";
	
	if (cmd == "on") {
		writeGPIO("value", "1");
	}
	if (cmd == "off") {
		writeGPIO("value", "0");
	}
	if (cmd=="blink") {
	if (number == "") {
	cout << "<div>Enter number of times to blink!!</div>" <<
	endl;
	}
	else {
	cout << "<div>Blinking LED attached to GPIO60 " << number << " times</div>";
	int count = stoi(number);

	for (int i = 0; i < count; i++){
	writeGPIO("value", "1");
	sleep(1);
	writeGPIO("value", "0");
	sleep(1);
	}
	}
	}
cout << body() << html();
return 0;
}
