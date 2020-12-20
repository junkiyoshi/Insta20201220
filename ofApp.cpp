#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofSetLineWidth(1.5);

	this->cap.open("D:\\MP4\\Pexels Videos 2441067.mp4");
	this->cap_size = cv::Size(128, 72);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			continue;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	ofRotateZ(ofGetFrameNum() * 0.5);

	ofMesh face, line;
	line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	float phi_deg_step = 15;
	float theta_deg_step = 9;
	for (int radius = 100; radius <= 300; radius += 100) {
		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg <= 180; theta_deg += theta_deg_step) {

				face.clear();
				line.clear();

				auto noise_value = ofNoise(glm::vec4(
					radius * sin(theta_deg * DEG_TO_RAD) * cos(phi_deg * DEG_TO_RAD) * 0.008,
					radius * sin(theta_deg * DEG_TO_RAD) * sin(phi_deg * DEG_TO_RAD) * 0.008,
					radius * cos(theta_deg * DEG_TO_RAD) * 0.008,
					ofGetFrameNum() * 0.005));

				if (noise_value > 0.55) {

					vector<glm::vec3> vertices;

					vertices.push_back(glm::vec3(
						radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
					vertices.push_back(glm::vec3(
						radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
					vertices.push_back(glm::vec3(
						radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
					vertices.push_back(glm::vec3(
						radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
						radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

					face.addVertices(vertices);
					line.addVertices(vertices);

					face.addTexCoord(glm::vec3(this->cap_size.width, 0, 0));
					face.addTexCoord(glm::vec3(0, 0, 0));
					face.addTexCoord(glm::vec3(this->cap_size.width, this->cap_size.height, 0));
					face.addTexCoord(glm::vec3(0, this->cap_size.height, 0));

					face.addIndex(0); face.addIndex(1); face.addIndex(3);
					face.addIndex(0); face.addIndex(3); face.addIndex(2);

					line.addIndex(0); line.addIndex(1);
					line.addIndex(0); line.addIndex(2);
					line.addIndex(3); line.addIndex(1);
					line.addIndex(3); line.addIndex(2);

					int n = int(ofMap(ofNoise(vertices[0] * 0.05), 0, 1, 0, this->number_of_frames) + ofGetFrameNum()) % this->number_of_frames;
					this->frame_list[n].copyTo(this->frame);
					this->image.update();

					ofSetColor(255);
					ofFill();
					this->image.bind();
					face.draw();
					this->image.unbind();

					ofSetColor(39);
					line.draw();
				}
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}