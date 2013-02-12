#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>

#include <shlobj.h>
#include <shlwapi.h>
#include <vector>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

//using namespace std;

#pragma comment(lib, "shlwapi.lib" )
#pragma once
#define BUF_SIZE 64

//�v���O�C�������N���X�����T���v���܂�܁B��ŕς���B
class Complex {
public:
	Complex() {
		setFileSize(-1);
		setNowSize(0);
		setNowPer(0);
		setCancelFlag(0);		
	}

	int download(std::string url, std::string savepath, std::string filename = ""){
    	char *domain;
    	char *path;
    	int port;
    	std::string p_domain;
    	std::string _domain;
    	std::string _path;

    	WSADATA wd;
    	SOCKET sock;
    	struct sockaddr_in server;
    	char buf[BUF_SIZE];
    	unsigned int **addrptr;

		if (filename != ""){
			savepath += "\\" + filename;//�f�t�H���g������npapi�ł͎g���Ȃ����ۂ��H
		}

    	//URL���(�v���g�R���A�h���C���A�p�X�A�|�[�g�̎擾)
    	int n= url.find("://");
    	if (n == -1) {
    		std::cout << "not found ://" << std::endl;
    		return -1;
    	}
    	p_domain = url.substr(n+3);
    	n = p_domain.find("/");
    	if (n == -1) {
    		std::cout << "not found /" << std::endl;
    		return -1;
    	}
    	_domain = p_domain.substr(0, n);
    	_path = p_domain.substr(n);
    	path = const_cast<char*>(_path.c_str());
    	n = _domain.find(":");
    	if (n == -1) {
    		domain = const_cast<char*>(_domain.c_str());
    		port = 80;
    	} else {
    		std::string tmp = _domain.substr(0, n);
    		domain = const_cast<char*>(tmp.c_str());
    		port = atoi(_domain.substr(n + 1).c_str());
    	}
    	//URL��͂����܂�

    	if ( WSAStartup(MAKEWORD(2,0), &wd) ) {//winsock������
    		//���������s
    		std::cout << "WSAStartup failed\n" <<  std::endl;
    		return -2;
    	}

    	sock = socket(AF_INET, SOCK_STREAM, 0);//�\�P�b�g�̍쐬
    	if (sock == INVALID_SOCKET) {
    		//�쐬���s
    		std::cout << "socket : " << WSAGetLastError() << std::endl;
			WSACleanup();
    		return -3;
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
    			}else {
    				std::cout << "gethostbyname failed" << std::endl;
    			}
				closesocket(sock);
				WSACleanup();
    			return -4;
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
				closesocket(sock);
				WSACleanup();
    			return -5;
    		}
    		
    	}

    	//�T�[�o�[�ɐڑ�
    	connect(sock, (struct sockaddr *)&server, sizeof(server));

    	//HTTP���N�G�X�g���M
    	memset(buf, 0, sizeof(buf));//buf������(�[���N���A)
    	sprintf_s(buf, sizeof(buf), "GET %s HTTP/1.1\r\n", path);
    	n = send(sock, buf, (int)strlen(buf), 0);
    	if (n < 0) {
    		//���M���s
    		std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
    		return -6;
    	}
    	sprintf_s(buf, sizeof(buf), "host: %s\r\n", domain);
    	n = send(sock, buf, (int)strlen(buf), 0);
    	if (n < 0) {
    		std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
			return -7;
    	}
    	sprintf_s(buf, sizeof(buf), "\r\n");
    	n = send(sock, buf, (int)strlen(buf), 0);
    	if (n < 0) {
    	std::cout << "send ; " << WSAGetLastError() << std::endl;
			closesocket(sock);
			WSACleanup();
    		return -8;
    	}
		//��C��send�o���Ȃ����ȁH�v����
		//�y�����z
		//���N�G�X�g������e�������Ŏ󂯎���đ����悤�ɂ���

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
    			return -9;
    		}
    		if (c == '\n'){
    			//1�s���̏���(source��1�s�̃f�[�^���i�[����Ă���)
    			if (source.size() < 2) {//source.empty()����_���������B��s�Ȃ̂�1�o�C�g�H�H
    				break;//�w�b�_�I��
    			} 
    			//�t�@�C���T�C�Y�擾
    			int k = source.find("Content-Length: ");
    			if (k != -1){
    				std::string _fsize = source.substr(k + 16);
    				setFileSize(atoi(_fsize.c_str()));
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
    	f.open(savepath, std::ios::out|std::ios::binary);
		//�y�����z
		//�������t�@�C���ƕ�����悤�Ȗ��O�t���Ċ����ナ�l�[��or
		//�ꎞ�ۑ���ɕۑ����ăt�@�C���ړ�������������
		#define BUF_TEST 256//�e�X�g�p�A�T�C�Y�m�肵����buf�g��
		char buf2[BUF_TEST];
		//buf2�̃T�C�Y��Sleep�̒l����肭�ݒ肵�Ȃ��ƃ_�E�����[�h�r���Ȃ̂�recv�̖߂�l���������Ȃ�H�H
    	while (1) {
    		memset(buf2, 0, sizeof(buf2));
    		n = recv(sock, buf2, sizeof(buf2), 0);
    		if (n < 0) {
    			std::cout << "recv : " << WSAGetLastError() << std::endl;
				f.close();
				//deleteFile(savepath);
				closesocket(sock);
				WSACleanup();
    			return -10;
    		}
    		
			Sleep(6);//msec�A�Ȃ����ꂪ�����Ə�肭�����Ȃ��̂��H
    		f.write(buf2, n);
			if (getCancelFlag() == 1) {
				break;
			}
    		if (n < BUF_TEST) {//n == 0���ƃt���[�Y����A��M�I�����肪�C�}�C�`�悭������Ȃ�
				std::cout << n << std::endl;
    			break;//��M����f�[�^�ʂ����Ȃ�������Ō�
    		}
			setNowSize(getNowSize() + BUF_TEST);//pNowSize += BUF_SIZE;
    	}
		setNowPer(2.0);
    	f.close();
		if (getCancelFlag() == 1){
			deleteFile(savepath);
		}
    	closesocket(sock);//�\�P�b�g�����
    	WSACleanup();//winsock�I��

    	return 1;
    }

	int DirExist(std::string path){
		return PathIsDirectory(path.c_str());
	}

	int makeDirs(std::string path){
		LPTSTR lp = NULL;
		char *fragment;
		std::vector<std::string> words;
		std::string tmp_dir;
		int words_size;

		if (DirExist(path)) {
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
			if (DirExist(tmp_dir)) {
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

	int deleteFile(std::string path, std::string filename = "") {
		if (filename != "") {
			path += "\\" + filename;
		}
		return DeleteFile(path.c_str());
	}

	int FileExist(std::string path, std::string filename = "") {
		if (filename != "") {
			path += "\\" + filename;
		}
		return PathFileExists(path.c_str());
	}

	std::string getNonFilename(std::string path, std::string filename){
		std::vector<std::string> words;
		std::string fname_name = "";
		std::string fname_ext;
		std::string fname_new;
		char fname_add[5];
		int fname_num = 1;
		int path_size = path.size();

		//�t�@�C�������݂��Ȃ���΂��̂܂܂̃t�@�C������Ԃ�
		if (path.find_last_of("\\") != path_size || path.find_last_of("/") != path_size) {
			//�p�X�̖������u\�v���́u/�v�Ŗ����ꍇ�͒ǉ�
			path += "\\";
		}
		if(FileExist(path + filename) == 0){
			std::cout << "OK. same file don't exists." << std::endl;
			return filename;
		}

		//fname��"."�ŕ�������words�Ɋi�[
		//��������------string.find��string.substr�g���Ă����ƃV���v���ɏ�������
		LPTSTR lptszNextToken = NULL;
		char *fragment = strtok_s(const_cast<char*>(filename.c_str()), ".", &lptszNextToken);
		while(fragment!= NULL){
			words.push_back(fragment);
			fragment = strtok_s(NULL, ".", &lptszNextToken);
		}

		int c = words.size();//�v�f��
		for(int k = 0; k < c-1; k++){
			fname_name += words[k];//������Ƃ��ĘA��(�g���q�͏���)
		}
		fname_ext = words[c-1];//�Ō�̗v�f(�g���q)
		//�����܂�------
		while(1){
			//�t�@�C�������d�����Ă�̂�(1)�̗l�ɖ����ɐ�����t��
			sprintf_s(fname_add, "(%d).", fname_num++);
			fname_new = fname_name + fname_add + fname_ext;
			if(FileExist(path + fname_new) == 0){
				break;
			}
		}
		return fname_new;//�d�����Ȃ��t�@�C������Ԃ�
	}

	std::string getDesktop(){
		//�f�X�N�g�b�v�p�X���擾
		std::string ret;
		char Path[256];
		SHGetSpecialFolderPath(NULL, Path, CSIDL_DESKTOP, FALSE);
		ret = Path;
		return ret;
	}

	std::string selectDir() {
		BROWSEINFO bi;
		char folder[260];
		LPITEMIDLIST idl;
		LPMALLOC g_pMalloc;
		SHGetMalloc(&g_pMalloc);

		//�c���[�^�C�v
		//�y�����z���O��t���ĕۑ��̂��~����
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

	//�ȉ��A�v���p�e�B�̎擾�Ɛݒ�
	float getNowPer(){
		if (pNowPer > 1.0) {
			return 2.0;//����
		}
		if (pFileSize == -1.0) {
			return -1.0;//�s��
		}
		return (float)pNowSize / pFileSize;//�i����
	}

	int getNowSize(){
		return pNowSize;
	}

	int getFileSize(){
		return pFileSize;
	}

	int getCancelFlag(){
		return pCancelFlag;
	}

	void setNowPer(float value){
		pNowPer = value;
	}

	void setNowSize(int value){
		pNowSize = value;
	}

	void setFileSize(int value){
		pFileSize = value;
	}

	void setCancelFlag(int value){
		pCancelFlag = value;
	}
private:
	float pNowPer;
	int pNowSize;
	int pFileSize;
	int pCancelFlag;
};
