#include "FileIO/TranslationParser.h"
#include "ApplicationState.h"
#include "Nodes/NodeTranslation.h"

#include <fstream>
#include <sstream>

#ifdef _WIN32
#include "dirent/dirent.h"
#else
#include <dirent.h>
#endif

TranslationParser::TranslationParser() {
}

TranslationParser::~TranslationParser() {
}

void TranslationParser::showParseMenu() {
	if (ImGui::BeginMenu("Parse SwissGerman translations")) {
		if (ImGui::Button("Parse OUT")) { doTranslationOut(); };
		ImGui::EndMenu();
	}
}

void TranslationParser::doTranslationOut() const {
	DIR* dir;
	struct dirent* de;

	dir = opendir(G_CONF.sqlFolder.c_str());
	int nr = 0;
	while (dir) {
		de = readdir(dir);
		if (!de) break;
		if (de->d_type == DT_DIR) {
			// we don't consider directory folders
			continue;
		}

		doTranslationOutFor(G_CONF.sqlFolder + "/" + de->d_name);
		
	}
	closedir(dir);
}

void TranslationParser::doTranslationOutFor(const std::string& file) const {
	std::vector<SwissTranslation> translations;
	std::string npcID;

	// read that file
	bool isStillOpen = false;
	std::string line;
	SwissTranslation currentTranslation;
	std::ifstream inputFile(file);
	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {
			if (line.empty() || line.compare(0, 2, "--") == 0) {
				continue;
			}
			if (isStillOpen) {
				while (true) {
					// this is a multiline translation, append everything until the next single '
					auto const nextDelimiter = line.find('\'');
					if (nextDelimiter == std::string::npos) {
						currentTranslation.swissGerman.append(line);
						currentTranslation.swissGerman.append("\n");
						break;
					}
					// is it an escaped apostroph?
					if (line.size() > nextDelimiter + 1 && line.at(nextDelimiter + 1) == '\'') {
						// it is 
						currentTranslation.swissGerman += line.substr(0, nextDelimiter + 2);
						line = line.substr(nextDelimiter + 2);
					} 
					else {
						// it isn't. Let's end this.
						currentTranslation.swissGerman += line.substr(0, nextDelimiter);
						translations.push_back(currentTranslation);
						isStillOpen = false;
						break;
					}
				}
				continue;
			}

			currentTranslation = SwissTranslation();

			// get the tag
			size_t nextDelimiter = line.find('\'');
			if (nextDelimiter == std::string::npos) {
				continue;
			}
			line = line.substr(nextDelimiter + 1);

			nextDelimiter = line.find('\'');
			if (nextDelimiter == std::string::npos) {
				continue;
			}

			currentTranslation.tag = line.substr(0, nextDelimiter);

			// get the npc tag
			nextDelimiter = line.find('\'');
			if (nextDelimiter == std::string::npos) {
				continue;
			}
			line = line.substr(nextDelimiter + 1);

			nextDelimiter = line.find('\'');
			if (nextDelimiter == std::string::npos) {
				continue;
			}
			line = line.substr(nextDelimiter + 1);

			nextDelimiter = line.find('\'');
			if (nextDelimiter == std::string::npos) {
				continue;
			}

			npcID = line.substr(0, nextDelimiter);

			bool notFound = false;

			for (int i = 0; i < 6;) {
				nextDelimiter = line.find('\'');
				if (nextDelimiter == std::string::npos) {
					notFound = true;
					break;
				}

				line = line.substr(nextDelimiter + 1);
				if (!line.empty() && line.at(0) == '\'') {
					line = line.substr(1);
				}
				else {
					i++;
				}
			}

			if (notFound) continue;

			while (true) {
				nextDelimiter = line.find('\'');
				if (nextDelimiter == std::string::npos) {
					isStillOpen = true;
					currentTranslation.swissGerman.append(line);
					currentTranslation.swissGerman.append("\n");
					break;
				}
				// is it an escaped apostroph?
				if (line.size() > nextDelimiter + 1 && line.at(nextDelimiter + 1) == '\'') {
					// it is 
					currentTranslation.swissGerman += line.substr(0, nextDelimiter + 2);
					line = line.substr(nextDelimiter + 2);
				}
				else {
					// it isn't. Let's end this.
					currentTranslation.swissGerman += line.substr(0, nextDelimiter);
					translations.push_back(currentTranslation);
					break;
				}
			}
		}

		inputFile.close();
	}
	else {
		ERROR("[Parser]: Unable to open input file.");
		return;
	}



	// now write that file
	std::string outPath = "db/swiss_translation/out/" + npcID + ".txt";
	std::ofstream outputFile(outPath, std::ios::trunc);
	if (outputFile.is_open()) {
		outputFile << npcID << '\n';
		outputFile << "----------------------------------\n";
		for (auto& trans : translations) {
			if (trans.tag.compare(0, 10, "DL_Cendric") == 0) {
				continue;
			}
			if (trans.tag.compare(0, 9, "DL_Choice") == 0) {
				continue;
			}

			outputFile << trans.tag << ":" << trans.swissGerman << "\n";
		}

		outputFile.close();
		LOG("[Parser] file successfully parsed and stored at: " + std::string(outPath));
	}
	else {
		ERROR("[Parser]: Unable to write to file: " + std::string(outPath));
	}
}
