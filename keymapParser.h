#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

#include "ckKey.h"

namespace KeymapParser {

	std::unordered_map<int, ckKey> parseFile(std::string fileName) {
		std::fstream stream = std::fstream(fileName, std::ios::in);
		std::unordered_map<int, ckKey> ret;

		int width = 0;
		float row = 0;
		float column = 0;
		std::vector<std::string> names;
		std::vector<float> sizes;
		bool multilineComment = false;

		std::string line;
		std::stringstream sstream = std::stringstream();
		while (std::getline(stream, line)) {
			if (line.length() <= 1) continue;

			std::string l2 = line;
			std::transform(l2.begin(), l2.end(), l2.begin(), toupper);
			
			// Skip comment
			if (l2.find("//") == 0) {
				continue;
			}
			else if (l2.find("/*") == 0) {
				multilineComment = true;
				continue;
			}
			else if (l2.find("*/") == 0) {
				multilineComment = false;
				continue;
			}
			else if (multilineComment)
				continue;


			// Find width
			if (l2.find("WIDTH") == 0) {
				l2 = l2.substr(l2.find('=') + 1);

				std::string::iterator end_pos = std::remove(l2.begin(), l2.end(), ' ');
				l2.erase(end_pos, l2.end());

				end_pos = std::remove(l2.begin(), l2.end(), ';');
				l2.erase(end_pos, l2.end());

				// Save width
				width = std::stoi(l2);
				
				names = std::vector<std::string>(width + 1);
				sizes = std::vector<float>(width + 1);
				continue;
			}

			// Ignore tab
			l2 = line.substr(l2.find('\t') == 0 ? l2.find('\t') + 1 : 0);

			int current = 0;

			// Handle key names
			if (l2.find(":") == 0) {
				l2 = l2.substr(1);	// Remove ":"
				names[0] = "";		// Reset first entry to avoid names carry over from last name line

				for (int i = 0; l2[i] != '\0'; i++){
					if (l2[i] == '\t' || l2[i] == ' ') {
						if (names[current] != "") {
							current++;
						}
					
						names[current] = "";
						continue;
					}

					names[current] += l2[i];
				}

				continue;
			}

			if (l2.find(";") == 0) {
				l2 = l2.substr(1);	// Remove ";"
				sizes[0] = 0.0f;	// Reset first entry to avoid names carry over from last name line

				std::string num = "";
				for (int i = 0; l2[i] != '\0'; i++){
					if (l2[i] == '\t' || l2[i] == ' ') {
						if (num != "") {
							sizes[current] = std::stof(num);
							num = "";

							current++;
						}
						continue;
					}

					num += l2[i];
				}

				continue;
			}

			// Remove whitespaces and tabs
			std::string::iterator end_pos = std::remove(l2.begin(), l2.end(), ' ');
			l2.erase(end_pos, l2.end());
			end_pos = std::remove(l2.begin(), l2.end(), '\t');
			l2.erase(end_pos, l2.end());

			// Parse key codes
			int keynumber = 0;
			while (l2.find("{") == 0) {
				std::string curr = l2.substr(1);
				curr.erase(l2.find("}") - 1);

				std::string sleft = curr.substr(0, curr.find(","));
				std::string sright = curr.substr(curr.find(",") + 1);
				
				int left = std::stoul(sleft, nullptr, 16);
				int right = std::stoul(sright, nullptr, 16);

				l2 = l2.erase(0, l2.find("}") + 1);
				l2 = l2.erase(0, l2.find(",") + 1);

				ret[left] = ckKey(left, right, column, row, sizes[keynumber], names[keynumber]);

				// Increment color in terms of key size
				column += sizes[keynumber++];
			}

			column = 0;
			row++;
		}

		return ret;
	}
}