#include <stdio.h>
#include <winsock2.h>
#include <shlwapi.h>
#include <Windows.h>
#include <CommDlg.h>

#include <shlobj.h>

#include <vector>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <process.h>

//using namespace std;

#pragma comment(lib, "shlwapi.lib" )
#pragma comment(lib,"shell32.lib")
#pragma once
#define BUF_SIZE 256

//�f�t�H���g������javascript����̌Ăяo���ł͎g�p�s��

//�錾
class Downloads;

//�X���b�h�̃p�����[�^�[��`
typedef struct _param{
	Downloads *ins;
	char *domain;
	char *rqpath;
	int port;
	std::string fullpath;
} param;

//�v���O�C�������N���X�����T���v���܂�܁B��ŕς���B
class Downloads {
public:
	// nyxysa�ŕ���class�������@��������Ȃ�
	Downloads(){
		//�R���X�g���N�^
		setFileSize(-1);
		setNowSize(0);
		setNowPer(0);
		setCancelFlag(0);
		setReferer("");
		setERROR("");
	}
	std::string post(std::string url, std::string postmsg){
		/**
		 * URL��POST����COOKIE���擾
		 * postmsg: ex) pixiv_id=(id)&pass=(pass)&mode=login&skip=1
		 */
		std::string cookie;
		char *domain;
		char *rqpath;
		int port;
		std::string p_domain;
		std::string _domain;
		std::string _path;

		WSADATA wd;
		SOCKET sock;
		struct sockaddr_in server;
		char buf[BUF_SIZE];
		unsigned int **addrptr;

		std::string fullpath;
		std::string tmpfile;

		int n= url.find("://");
		if (n == -1) {
    		std::cout << "not found ://" << std::endl;
    		return "";
		}
		p_domain = url.substr(n+3);
		n = p_domain.find("/");
		if (n == -1) {
    		std::cout << "not found /" << std::endl;
    		return "";
		}
		_domain = p_domain.substr(0, n);
		_path = p_domain.substr(n);
		rqpath = const_cast<char*>(_path.c_str());
		n = _domain.find(":");
		if (n == -1) {
    		domain = const_cast<char*>(_domain.c_str());
    		port = 80;
		} else {
    		std::string tmp = _domain.substr(0, n);
    		domain = const_cast<char*>(tmp.c_str());
    		port = atoi(_domain.substr(n + 1).c_str());
		}

		if ( WSAStartup(MAKEWORD(2,0), &wd) ) {
    		std::cout << "WSAStartup failed\n" <<  std::endl;
    		return "";
		}

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
    		std::cout << "socket : " << WSAGetLastError() << std::endl;
			WSACleanup();
    		return "";
		}

		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.S_un.S_addr = inet_addr(domain);
		if (server.sin_addr.S_un.S_addr == 0xffffffff) {
      		struct hostent *host;
    		host = gethostbyname(domain);
    		if (host == NULL) {
    			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
    				std::cout << "host not found : " << domain << std::endl;
    			}else {
    				std::cout << "gethostbyname failed" << std::endl;
    			}
				closesocket(sock);
				WSACleanup();
    			return "";
    		}
    		addrptr = (unsigned int **)host->h_addr_list;
    		while (*addrptr != NULL) {
    			server.sin_addr.S_un.S_addr = *(*addrptr);
    			if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
    				break;
    			}
    			addrptr++;
    		}
    		if (*addrptr == NULL) {
    			std::cout << "connect : " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
    			return "";
    		}
		}

		connect(sock, (struct sockaddr *)&server, sizeof(server));

		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, sizeof(buf), "POST %s HTTP/1.1\r\n", rqpath);//���\�b�h��POST��
		send(sock, buf, (int)strlen(buf), 0);
		sprintf_s(buf, sizeof(buf), "host: %s\r\n", domain);
		send(sock, buf, (int)strlen(buf), 0);
		sprintf_s(buf, sizeof(buf), "\r\n");
		send(sock, buf, (int)strlen(buf), 0);
		sprintf_s(buf, sizeof(buf), postmsg.c_str());//��s�̌�Ƀ|�X�g���b�Z�[�W�𑗐M
		send(sock, buf, (int)strlen(buf), 0);

		std::string source = "";
		while (1) {
    		char c;
    		n = recv(sock, &c, 1, 0);
    		if (n == SOCKET_ERROR) {
    			std::cout << "recv : " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
    			return "";
    		}
    		if (c == '\n'){
    			if (source.size() < 2) {
	   				break;
    			} 
    			std::cout << source << std::endl;
				std::string::size_type k1, k2;
				k1 = source.find("Set-Cookie: ");//�N�b�L�[�̎擾
				k2 = source.find(";");
    			if (k1 != -1){
					closesocket(sock);
					WSACleanup();
 					cookie = source.substr(k1 + 12, k2 - (k1 + 12));
					std::cout << cookie <<std::endl;
					addCookie(cookie);
   					return cookie;//����
    			}
    			std::cout << "--------------------" << std::endl;
    			source = "";
    			continue;
    		}
    		source += c;
		}
		//���s
		closesocket(sock);
		WSACleanup();
		return "";
	}

	//�_�E�����[�h�֘A
	void download(std::string url, std::string path, std::string filename, int ask=0){
		/**
		 * �_�E�����[�h
		 *
		 * filename���󕶎��ɂ���path�Ƀt���p�X�ł�OK
		 * ask=1�Ŗ��O��t���ĕۑ��_�C�A���O��\��
		 * �@���̍ہApath, filename�͕ۑ��_�C�A���O�Ńf�t�H���g�p�X�A�f�t�H���g�t�@�C�����Ƃ��ė��p
		 *
		 * �Z�L�����e�B�΍�F
		 *   �E�㏑���ۑ����͊m�F�_�C�A���O���o��
		 *   �E�댯�ȃp�X(�X�^�[�g�A�b�v�t�H���_�Ȃ�)�ւ̕ۑ��͕s��
		 */
		std::string domain, rqpath, fullpath;
		int port;
		std::string _domain;

		if (ask == 1) {
			//�ۑ�������[�U�[�Ɋm�F����ꍇ
			fullpath = saveFileDialog(path, filename);
			if (path.empty()) {
				std::cout << "�_�E�����[�h�̓L�����Z������܂���" << std::endl;
				return;
			}
			std::string::size_type n = fullpath.find_last_of("\\");
			path = fullpath.substr(0, n);
			filename = fullpath.substr(n + 1, fullpath.size() - (n + 1));
		} else {
			//�ۑ�������܂��Ă�ꍇ
			if (filename.empty()){
				//path�Ƀt���p�X��n���Afilename���ȗ������ꍇ
				fullpath = path;
				std::string::size_type n = fullpath.find_last_of("\\");
				if (n == std::string::npos) {
					n = fullpath.find_last_of("/");
					if (n == std::string::npos) {
						std::cout << "�����̒l���s���ł�" << std::endl;
						return;
					}
				}
				path = fullpath.substr(0, n);
				filename = fullpath.substr(n + 1, fullpath.size() - (n + 1));
			} else {
				//path��filename�𕁒ʂɎw�肵���ꍇ
				fullpath = path + "\\" + filename;
			}

			//�㏑���`�F�b�N
			if (fileExist(path, filename)) {
				//�㏑���ɂȂ�ꍇ�͊m�F�_�C�A���O���o��
				std::cout << "�t�@�C�����d�����Ă��܂�" << std::endl;
				int n = askDialog(fullpath + "���㏑�����܂����H", "�m�F", 0);
				if (n < 0) {
					//�u�L�����Z���v�I���Ń_�E�����[�h���~
					std::cout << "�_�E�����[�h�̓L�����Z������܂���" << std::endl;
					return;
				}else if (n == 0) {
					//�u�������v�I���͕ʂ̖��O�ŕۑ�(���O��t���ĕۑ��_�C�A���O�Ăяo��)
					fullpath =saveFileDialog(path, getNonFilename(path, filename));
					if (fullpath.empty()) {
						//���O��t���ĕۑ��_�C�A���O�Łu�L�����Z���v��(�_�E�����[�h�L�����Z��)
						std::cout << "�_�E�����[�h�̓L�����Z������܂���" << std::endl;
						return;
					}
				}
			}
		}

		//�ۑ���̈��S�����m�F
		if (securityCheck(path) == 0) {
			std::cout << "�ۑ��悪�댯�ȃp�X�ł�" << std::endl;
			return;
		}
		//�ۑ���A�t�@�C�������߁@�����܂�
		std::cout << "fullpath: " << fullpath << std::endl;

		//URL���(�v���g�R���A�h���C���A�p�X�A�|�[�g�̎擾)
		int n= url.find("://");
		if (n == -1) {
    		std::cout << "not found ://" << std::endl;
    		return;
		}
		_domain = url.substr(n+3);
		n = _domain.find("/");
		if (n == -1) {
    		std::cout << "not found /" << std::endl;
    		return;
		}
		domain = _domain.substr(0, n);
		rqpath = _domain.substr(n);
		n = domain.find(":");
		if (n == -1) {
			//URL�Ƀ|�[�g�ԍ����܂܂�ĂȂ��ꍇ(�|�[�g80)
    		port = 80;
		} else {
			//URL�Ƀ|�[�g�ԍ����܂܂�Ă�ꍇ
    		domain = domain.substr(0, n);
    		port = atoi(_domain.substr(n + 1).c_str());
		}
		//URL��͂����܂�
		std::cout << "domain: " << domain << ", rqpath: " << rqpath << ", port: " << port << std::endl;

		//�X���b�h�̃p�����[�^�̐ݒ�
		param p = { this, const_cast<char*>(domain.c_str()), const_cast<char*>(rqpath.c_str()), port, fullpath};
		//�X���b�h�̋N��
		h = (HANDLE)_beginthread(downloading, 0, &p);

		//���ꂪ�����ƃN���b�V������
		//�ł����ꏑ���ƃ}���`�X���b�h�ɂȂ�Ȃ��c
		WaitForSingleObject(h, INFINITE);
		CloseHandle(h);
		return;
	}
	int getFileSize(){ return fileSize; }
	int getNowSize(){ return nowSize; }
	float getNowPer(){
		if (nowPer > 1.0) return 2.0;
		if (fileSize == -1.0) return -1.0;
		return (float)nowSize / fileSize;
	}
	int getCancelFlag(){ return cancelFlag; }
	std::string getCookie(){
		std::string ret = "";
		int n = cookies.size();
		for(int i = 0; i < n ; i++){
			ret += cookies[i] + ";";
		}
		return ret;
	}
	std::string getReferer(){ return referer; }
	std::string getERROR(){ return error; }
	void setFileSize(int value){ fileSize = value; }
	void setNowSize(int value){ nowSize = value; }
	void setNowPer(float value){ nowPer = value; }
	void setCancelFlag(int value){ cancelFlag = value; }
	void addCookie(std::string value){ cookies.push_back(value); }
	void clearCookie(){ cookies.clear(); }
	void setCookie(std::string value){
		LPSTR lp = NULL;
		clearCookie();
		char *fragment = strtok_s(const_cast<char*>(value.c_str()), ";", &lp);
		while(fragment!= NULL){
			addCookie(fragment);
			fragment = strtok_s(NULL, "\\", &lp);
		}
	}
	void setReferer(std::string value){ referer = value; }
	void setERROR(std::string value){ error = value; }

	//�_�C�A���O�֘A
	std::string selectDirDialog() {
		/**
		 * �f�B���N�g���I���_�C�A���O�̌Ăяo��
		 * �c���[�^�C�v(�ݒ�p)
		 */
		BROWSEINFO bi;
		char folder[260];
		LPITEMIDLIST idl;
		LPMALLOC g_pMalloc;
		SHGetMalloc(&g_pMalloc);

		memset(&bi, 0, sizeof(BROWSEINFO));
		bi.hwndOwner = NULL;
		bi.pszDisplayName = folder;
		bi.lpszTitle = TEXT("�t�H���_��I�����Ă�������");
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		idl = SHBrowseForFolder(&bi);
		if(idl){
			int b = SHGetPathFromIDList(idl, folder);
			if(b){
				return (std::string)folder;
			}
		}
		g_pMalloc->Free(idl);
		return "";
	}
	std::string saveFileDialog(std::string path, std::string filename) {
		/**
		 * ���O�����ĕۑ��_�C�A���O�̌Ăяo��
		 * (�㏑���ۑ��̊m�F����)
		 *
		 * �߂�l�F�t�@�C���̃t���p�X, �L�����Z�����͋󔒕���
		 */
		static OPENFILENAME ofn;
		static TCHAR szFile[MAX_PATH];

		char *tmp = const_cast<char*>(filename.c_str());
		strncpy_s(szFile, sizeof(szFile), tmp, strlen(tmp));
	
		memset(&ofn, 0, sizeof(OPENFILENAME));
		//�_�C�A���O�̐ݒ�
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrInitialDir = const_cast<char*>(path.c_str());//�����t�H���_
		ofn.lpstrFile = szFile;//���ʂ��󂯎��o�b�t�@�A�����t�@�C����
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = TEXT("���O��t���ĕۑ�");
		ofn.lpstrFilter = TEXT("���ׂẴt�@�C��(*.*)");
		//ofn.lpstrDefExt = TEXT(".jpg");
		ofn.Flags = OFN_OVERWRITEPROMPT;//�㏑������ꍇ�A�m�F�_�C�A���O���o��

		//�_�C�A���O�̌Ăяo��
		if ( GetSaveFileName(&ofn) ){
			std::cout << "szFile : " << szFile << std::endl;
			return szFile;//char*�^��string�^�L���X�g���Ă����
		} 
		//�L�����Z���{�^�����������ꍇ
		std::cout << "miss" << std::endl;
		return "";
	}
	int alertDialog(std::string msg){
		/**
		 * �x���_�C�A���O�̌Ăяo��
		 */
		return MessageBox(NULL, msg.c_str(), "�x��", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	}
	int askDialog(std::string msg, std::string title, int flag){
		/**
		 * �m�F�_�C�A���O(�͂��A�������A�L�����Z��)�̌Ăяo��
		 * 
		 * ����: flag) 1 = �͂�or������, 0 = �͂�or������or�L�����Z��
		 * �߂�l:1 = �͂�, 0 = ������, -1 = �L�����Z��
		 * 
		 * ex) �ۑ��̊m�F (�t�@�C����)�͊��ɑ��݂��܂��B\n�㏑�����܂����H
		 */
 
		int res, tmp;
		if (flag == 0) {
			tmp = MB_YESNOCANCEL | MB_ICONWARNING | MB_SETFOREGROUND;
		} else {
			tmp = MB_YESNO | MB_ICONWARNING | MB_SETFOREGROUND;
		}
		res = MessageBox(NULL, msg.c_str(), title.c_str(), tmp);

		switch(res){
		case IDYES:
			return 1;
		case IDNO:
			return 0;
		case IDCANCEL:
			return -1;
		default:
			return -2;
		}
	}

	//�p�X�֘A
	int dirExist(std::string path){
		/**
		 * �w�肵���p�X�̃f�B���N�g�������݂��邩�m�F
		 */
		return PathIsDirectory(path.c_str());
	}
	int makeDirs(std::string path){
		/**
		 * �ċA�I�ȃf�B���N�g������
		 * �߂�l�F0=���Ƀf�B���N�g��������, -1=�������s
		 */
		LPTSTR lp = NULL;
		char *fragment;
		std::vector<std::string> words;
		std::string tmp_dir;
		int words_size;

		if (dirExist(path)) {
			std::cout << "DIR EXIST" << std::endl;//���ɂ���
			return 0;
		}

		if (CreateDirectory(path.c_str(), NULL)) {
			std::cout << "CREATE DIR" << std::endl;//���ʂɍ���
			return 1;
		}

		//�ċA�I�ȃf�B���N�g������
		fragment = strtok_s(const_cast<char*>(path.c_str()), "\\", &lp);
		while(fragment!= NULL){
			words.push_back(fragment);
			fragment = strtok_s(NULL, "\\", &lp);
		}
		tmp_dir = words[0];
		words_size = words.size();
		for (int i = 1; i < words_size; i++) {
			tmp_dir += "\\" + words[i];
			if (dirExist(tmp_dir)) {
				continue;
			}
			if (CreateDirectory(tmp_dir.c_str(), NULL) == 0) {
				//���Ԃ�h���C�u�����݂��Ȃ��Ƃ��������ݕs�ȏꏊ�Ƃ�
				std::cout << "can't create dir : " << tmp_dir << std::endl;
				return -1;
			}
			std::cout << tmp_dir << std::endl;
		}

		return 1;
	}
	int fileExist(std::string path, std::string filename = "") {
		/**
		 * �w�肵���p�X�Ɏw�肵���t�@�C�������݂��邩�m�F
		 * filename���ȗ�����path�Ƀt���p�X��n���Ă�OK
		 */
		if (filename != "") {
			path += "\\" + filename;
		}
		return PathFileExists(path.c_str());
	}
	std::string getNonFilename(std::string path, std::string filename){
		/**
		 * �d�����Ȃ�(�㏑���ɂȂ�Ȃ�)�t�@�C������Ԃ�
		 * �����̃t�@�C�������݂���ꍇ�A������(����)��t������
		 *   ex) filename(1).jpg
		 */
		std::vector<std::string> words;
		std::string fname_name = "";
		std::string fname_ext;
		std::string fname_new;
		char fname_add[5];//�u(99)\0�v��5����(������2���܂�)
		int fname_num = 1;
		int path_size = path.size();

		//�t�@�C�������݂��Ȃ���΂��̂܂܂̃t�@�C������Ԃ�
		if (path.find_last_of("\\") != path_size || path.find_last_of("/") != path_size) {
			//�p�X�̖������u\�v���́u/�v�Ŗ����ꍇ�͒ǉ�
			path += "\\";
		}
		if(fileExist(path, filename) == 0){
			std::cout << "OK. same file don't exists." << std::endl;
			return filename;
		}

		//�t�@�C�����Ɗg���q�𒲂ׂ�
		std::cout << filename << std::endl;
		std::string::size_type n = filename.find_last_of(".");
		if (n != std::string::npos) {
			fname_name = filename.substr(0, n);
			fname_ext = filename.substr(n + 1, filename.size() - (n) + 1);
		} else {
			std::cout << "�g���q��������܂���ł���" << std::endl;
			return "";
		}

		while(1){
			//�t�@�C�������d�����Ă�̂�(1)�̗l�ɖ����ɐ�����t��
			sprintf_s(fname_add, "(%d).", fname_num++);
			fname_new = fname_name + fname_add + fname_ext;
			if(fileExist(path, fname_new) == 0){
				break;
			}
		}
		return fname_new;//�d�����Ȃ��t�@�C������Ԃ�
	}
	std::string getDesktop(){
		/**
		 * �f�X�N�g�b�v�p�X���擾
		 */
		std::string ret;
		char Path[256];
		SHGetSpecialFolderPath(NULL, Path, CSIDL_DESKTOP, FALSE);
		ret = Path;//char*�^ �� string�^�L���X�g
		return ret;
	}
	int securityCheck(std::string _path){
		/**
			* �X�^�[�g�A�b�v�t�H���_�y�уp�X�̒ʂ����t�H���_(�V�X�e���t�H���_��)��
			* �_�E�����[�h�o���Ȃ��悤�ɂ��āA�Z�L�����e�B�̋�����}��
			* 
			* �߂�l�F1 => ���S�ȃp�X, 0 => �댯�ȃp�X
			*/
		char startup[MAX_PATH];
		char *path = const_cast<char*>(_path.c_str());
		char *p;
		LPTSTR lptszNextToken = NULL;
		char *env;
		char *fragment;
		size_t requiredSize;

		SHGetSpecialFolderPath( NULL, startup, CSIDL_STARTUP, FALSE );//�ʃ��[�U�[�̃X�^�[�g�A�b�v
		//std::cout << startup << std::endl;
		p = strstr(path, startup);
		if (NULL != p) {
			std::cout << "Alert: startup directory" <<std::endl;
			alertDialog("�X�^�[�g�A�b�v�t�H���_�Ƀt�@�C����ۑ����悤�Ƃ��܂����B\n�_�E�����[�h�𒆎~���܂��B");
			return 0;
		}

		SHGetSpecialFolderPath( NULL, startup, CSIDL_COMMON_STARTUP, FALSE );//�S�Ẵ��[�U�[�̃X�^�[�g�A�b�v
		//std::cout << startup << std::endl;
		p = strstr(path, startup);
		if (NULL != p) {
			std::cout << "Alert: startup directory(all users)" << std::endl;
			alertDialog("�X�^�[�g�A�b�v�t�H���_(All Users)�Ƀt�@�C����ۑ����悤�Ƃ��܂����B\n�_�E�����[�h�𒆎~���܂��B");
			return 0;
		}

		//���ϐ�PATH�̎擾
		getenv_s(&requiredSize, NULL, 0, "Path");
		env = (char*) malloc(requiredSize * sizeof(char));
		getenv_s(&requiredSize, env, requiredSize, "Path");

		//���ϐ�PATH�̃f�B���N�g�����P�����ׂ�
		fragment = strtok_s(env, ";", &lptszNextToken);
		while(fragment!= NULL){
			//std::cout << fragment << std::endl;
			p = strstr(path, fragment);
			if (NULL != p) {
				std::cout << "Alert : system environment variable �uPATH�v directory" << std::endl;
				std::string msg = fragment;
				msg += "\n�Ƀt�@�C����ۑ����悤�Ƃ��܂����B\n�_�E�����[�h�𒆎~���܂��B";
				alertDialog(msg);
				free(env);
				return 0;
			}
			fragment = strtok_s(NULL, ";", &lptszNextToken);
		}
		free(env);
		return 1;
	}
	int openExplore(std::string path){
		/**
		 * �G�N�X�v���[���[�Ŏw�肵���p�X���J��
		 */
		//HINSTANCE n = ShellExecute(NULL, "explore", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return 1;
	}

	//�t�@�C������֘A
	//�Z�L�����e�B�΍�
	//�t�@�C���̍폜�E���l�[����javascript���璼�ڌĂяo���Ȃ��悤��
	//�uComplex.idl�v�t�@�C���ɂ͋L�q���Ȃ�
	//(downloading�֐�����Ăяo���̂�private�ɂ͂��Ȃ�)
	static int deleteFile(std::string path, std::string filename = "") {
		/**
		 * �t�@�C���̍폜
		 * filename���ȗ�����path�Ƀt���p�X��n���Ă�OK
		 */
		if (!filename.empty()) {
			path += "\\" + filename;
		}
		return DeleteFile(path.c_str());
	}
	static int renameFile(std::string before, std::string after, std::string path=""){
		/**
		 * �t�@�C���̃��l�[��
		 * before:���l�[���O�̃t�@�C����, after:���l�[����̃t�@�C����, path:�t�@�C���̂���ꏊ
		 * �܂��Apath���ȗ�����before, after�Ƀt���p�X��n���Ă�OK
		 * (���̍�before��after�̃f�B���N�g�����Ⴆ�΃t�@�C���̈ړ����s��)
		 */
		if (!path.empty()) {
			before = path + "\\" + before;
			after  = path + "\\" + after;
		}
		if (rename(before.c_str(), after.c_str()) == 0) {
			std::cout << before << "��" << after << "�Ƀ��l�[������" << std::endl;
			return 1;
		} else {
			std::cout << "���l�[�����s" << std::endl;
			return 0;
		}
	}
private:
	int fileSize;
	int nowSize;
	float nowPer;
	int cancelFlag;
	std::vector<std::string> cookies;
	std::string referer;
	HANDLE h;
	std::string error;

	//�_�E�����[�h�̃X���b�h
	static void downloading(void* vp){
		/**
		 * �_�E�����[�h(�}���`�X���b�h�p�ɕ���)
		 *   javascript����Ăяo���s��
		 */
		//�p�����[�^�����񔲂��o��
		param p = *((param*)vp);
		Downloads *ins = p.ins;
		char *domain = p.domain;
		char *rqpath = p.rqpath;
		int port = p.port;
		std::string fullpath = p.fullpath;
		std::string tmpfile = fullpath + ".download";

		WSADATA wd;
		SOCKET sock;
		struct sockaddr_in server;
		char buf[BUF_SIZE];
		unsigned int **addrptr;

		if ( WSAStartup(MAKEWORD(2,0), &wd) ) {//winsock������
    		//���������s
    		std::cout << "WSAStartup failed\n" <<  std::endl;
			ins->setERROR("WSAStartup failed");
    		return;
		}

		sock = socket(AF_INET, SOCK_STREAM, 0);//�\�P�b�g�̍쐬
		if (sock == INVALID_SOCKET) {
    		//�쐬���s
    		std::cout << "socket : " << WSAGetLastError() << std::endl;
			ins->setERROR("socketmake failed ");
			WSACleanup();
    		return;
		}
		//�ڑ���w��p�\���̂̏���
		server.sin_family = AF_INET;//�C���^�[�l�b�g(TCP, UDP, etc...)
		server.sin_port = htons(port);//http�|�[�g80
		server.sin_addr.S_un.S_addr = inet_addr(domain);//URL�ݒ�(inet_addr��32bit�o�C�i���ւ̕ϊ�)
		if (server.sin_addr.S_un.S_addr == 0xffffffff) {
    		//URL��IP�\�L����Ȃ��ꍇ(���O�������K�v�ȏꍇ)
    		struct hostent *host;
    		host = gethostbyname(domain);//���O����(��IP)
    		if (host == NULL) {
    			//�������s
    			if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
    				std::cout << "host not found : " << domain << std::endl;
					ins->setERROR("host not found");
    			}else {
    				std::cout << "gethostbyname failed" << std::endl;
					ins->setERROR("gethostbyname failed");
    			}
				closesocket(sock);
				WSACleanup();
    			return;
    		}
    		addrptr = (unsigned int **)host->h_addr_list;
    		while (*addrptr != NULL) {
    			//1�̖��O�ɕ�����IP���֘A�t�����Ă��邱�Ƃ�����̂Őڑ�����������܂őS�Ď���
    			server.sin_addr.S_un.S_addr = *(*addrptr);
    			if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
    				break;//�ڑ�����
    			}
    			addrptr++;
    		}
    		if (*addrptr == NULL) {
    			//�S�Ă�IP�Őڑ������s
    			std::cout << "connect : " << WSAGetLastError() << std::endl;
				ins->setERROR("connect failed");
				closesocket(sock);
				WSACleanup();
    			return;
    		}
    		
		}

		//�T�[�o�[�ɐڑ�
		connect(sock, (struct sockaddr *)&server, sizeof(server));

		//HTTP���N�G�X�g���M
		memset(buf, 0, sizeof(buf));//buf������(�[���N���A)
		sprintf_s(buf, sizeof(buf), "GET %s HTTP/1.1\r\n", rqpath);
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
    		//���M���s
    		std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
			ins->setERROR("http request send failed");
    		return;
		}
		sprintf_s(buf, sizeof(buf), "host: %s\r\n", domain);
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
    		std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
			ins->setERROR("http request send failed");
			return;
		}
		if (!ins->getCookie().empty()) {
			//�N�b�L�[�̐ݒ�
			std::string cookie = "Cookie: " + ins->getCookie() + "\r\n";
			std::cout << cookie << std::endl;
			sprintf_s(buf, sizeof(buf), cookie.c_str());
			send(sock, buf, (int)strlen(buf), 0);

		}
		if (!ins->getReferer().empty()) {
			//���t�@���̐ݒ�
			std::string ref = "Referer: " + ins->getReferer() + "\r\n";
			sprintf_s(buf, sizeof(buf), ref.c_str());
			send(sock, buf, (int)strlen(buf), 0);
		}
		sprintf_s(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
		std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
			ins->setERROR("http request send failed");
    		return;
		}

		//�T�[�o�[����f�[�^��M(�w�b�_)
		std::string source = "";
		while (1) {
    		//c��1�������擾����source�ɒǋL���ĉ��s��������source�N���A
    		char c;
    		n = recv(sock, &c, 1, 0);
    		if (n == SOCKET_ERROR) {
    			//��M���s
    			std::cout << "recv : " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				ins->setERROR("recv failed");
    			return;
    		}
    		if (c == '\n'){
    			//1�s���̏���(source��1�s�̃f�[�^���i�[����Ă���)
    			if (source.size() < 2) {
					//�w�b�_�Ǝ��f�[�^�̊Ԃ͋�s
					//�ł�source.empty()����_���B�T�C�Y��1(NULL����'\0'�������Ă邩��H)
    				break;//�w�b�_�I��
    			}
				//�X�e�[�^�X�擾
				std::string::size_type k = source.find("HTTP/1.1 ");
				if (k != std::string::npos) {
					std::string status = source.substr(k + 9, source.size() - (k + 9));	
					int _status = atoi(status.c_str());
					if (_status != 200) {
						//400 Bad Request
						//403 Forbidden
						//404 Not Found�ȂǂȂ�
						std::cout << "Status: " << status << std::endl;
						closesocket(sock);
						WSACleanup();
						ins->setERROR(status);
						return;
					}
				}
    			//�t�@�C���T�C�Y�擾
    			k = source.find("Content-Length: ");
				if (k != std::string::npos) {
    				std::string _fsize = source.substr(k + 16);
					ins->setFileSize(atoi(_fsize.c_str()));
					std::cout << "filesize: " << ins->getFileSize() << std::endl;
    			}
    			std::cout << source << std::endl;
    			std::cout << "------" << source.length() << "--------------" << std::endl;
    			source = "";//���̍s���擾���邽�߂ɃN���A
    			continue;
    		}
    		source += c;//���s������܂�source�ɒ~��
		}

		//�T�[�o�[����f�[�^��M(���f�[�^)
		std::ofstream f;
		f.open(tmpfile, std::ios::out|std::ios::binary);//�_�E�����[�h����.download���t�@�C���������ɕt��
		//�y�����z
		while (1) {
    		memset(buf, 0, sizeof(buf));
    		n = recv(sock, buf, sizeof(buf) - 1, 0);//NULL����Œǉ����镪1�o�C�g����������
			std::cout << ".";
    		if (n < 0) {
    			std::cout << "recv : " << WSAGetLastError() << std::endl;
				f.close();
				ins->deleteFile(tmpfile);
				closesocket(sock);
				WSACleanup();
				ins->setERROR("recv failed");
    			return;
    		}  		
			buf[n] = '\0';//NULL�����t��
    		f.write(buf, n);
			if (ins->getCancelFlag() == 1) {
				std::cout << "���[�U�[�ɂ��L�����Z������܂���" << std::endl;
				break;
			}
			ins->setNowSize(ins->getNowSize() + n);//nowSize += n;
			if (n <= 0 || ins->getNowSize() >= ins->getFileSize()) {
				//��M�T�C�Y��0����
				//�_�E�����[�h�T�C�Y���t�@�C���T�C�Y�ɓ��B������I��
				std::cout << "\nn(<=0) : " << n << std::endl;
    			break;
    		}
		}
		std::cout << "now size(end) : " << ins->getNowSize() << std::endl;
		std::cout << "file size     : " << ins->getFileSize() << std::endl;
		f.close();
		closesocket(sock);//�\�P�b�g�����
		WSACleanup();//winsock�I��
		if (ins->getCancelFlag() == 1){
			//�L�����Z����
			ins->deleteFile(tmpfile, "");
			ins->setERROR("cancel");
			return;
		}
 		//�_�E�����[�h����
		ins->setNowPer(2.0);
		if(ins->fileExist(fullpath, "")) {
			//�㏑������Ȃ�t�@�C���폜���Ƃ��Ȃ��ƃ��l�[�����s����
			ins->deleteFile(fullpath, "");
		}
		ins->renameFile(tmpfile, fullpath, "");//�u.download�v����
		return;
	}
};


