Download NPAPI Plugin for chrome, Windows
=========================================
Google Chrome�g���ׂ̈�Windows��pNPAPI�v���O�C���ł��B  
javascript����v���O�C�����Ăяo���ă_�E�����[�h�y�т��̊֘A�@�\�𗘗p�ł��܂��B  

����m�F�Fwindows7 64bit, Google Chrome 26.0.1403.0 dev-m  

���\�[�X�R�[�h�̃R���p�C��
---------------------
�\�[�X�R�[�h�̃R���p�C�����@�ɂ��Ă�readme_src.md��ǂ�ł��������B

���g����
--------
**�`�v���O�C���̌Ăяo���`** 
###�Emanifest.json###
    "plugins" : [
        { "path": "npcomplex.dll", "public": true}
    ],

�����ۂɎg�p����ۂ�public��true���ƃZ�L�����e�B�I�ɖ��Ȃ̂Œ���

###�Ehtml�t�@�C��###
    <object id="pluginId" type="application/complex" style="width: 0; height: 0;">
    Plugin FAILED to load
    </object>

###�Ejs�t�@�C��###
    pluginelm = document.getElementById("pluginId");
    c = pluginelm.Complex();
    c.download(url,savepath,filename);

**�`�e���\�b�h�̐����`**

###�E�_�E�����[�h�̎��s###
    download(url, savepath, filename);

+   `url` :  
�@�@ _�_�E�����[�h����t�@�C����URL_

+   `savepath` :  
�@�@ _�ۑ���̃p�X_

+   `filename` �F  
�@�@ _�ۑ�����t�@�C����_

+   `�߂�l` �F  
�@�@ _1(����)or-1�`-10(���s)_

�������̃t�@�C�����_�E�����[�h����ꍇ�͕K�����L�̂悤�ɃC���X�^���X�𕪂��Ă��������B  
�_�E�����[�h�󋵂̊m�F�A�L�����Z�������o���Ȃ��Ȃ�܂��B
###�i�C���X�^���X�𕪂���j####
    var c1 = pluginelm.Complex();
    var c2 = pluginelm.Complex();
    c1.download(url1, savepath1, filename1);
    c2.download(url2, savepath2, filename2);

###�E�_�E�����[�h����t�@�C���̃T�C�Y�̎擾###
    getFileSize();
+   `�߂�l` �F  
�@�@ _�t�@�C���̃T�C�Y(�w�b�_��Content-Length :�`�̕���)_

###�E�_�E�����[�h�󋵂̊m�F(Bytes)###
    getNowSize();
+   `�߂�l` �F  
�@�@ _�t�@�C���̃T�C�Y(256�o�C�g���ɍX�V)_

###�E�_�E�����[�h�󋵂̊m�F(��)###
    getNowPer();
+   `�߂�l` �F  
�@�@ _�_�E�����[�h�󋵂�float�ŕԂ�(��2������������)_  
�@�@ _�t�@�C���T�C�Y���擾�̏ꍇ��-1.0_  
�@�@ _�_�E�����[�h�����ς݂̏ꍇ��2.0��Ԃ�_

###�E�_�E�����[�h�̃L�����Z��###
    setCancelFlag(1);

###�E�p�r�s��###
    setFileSize(size);
    setNowSize(size);
    setNowPer(size);
    getCancelFlag();

**�`���̑��̃��\�b�h�̐����`**

###�E�f�B���N�g���̑��݊m�F###
    DirExist(path);

+   `path` :  
�@�@ _�f�B���N�g���̃p�X_

+   `�߂�l` �F  
�@�@ _1(�L)or0(��)_  
�@�@ _<�P��PathIsDirectory�̖߂�l��Ԃ�>_

###�E�f�B���N�g���̐���(�ċA�I)###
    makeDirs(path);

+   `path` :  
�@�@ _�����������f�B���N�g�����܂߂��t���p�X_

+   `�߂�l` �F  
�@�@ _1(����)or0(���s)_

###�E�t�@�C���̍폜###
    deleteFile(path, filename);

+   `path` :  
�@�@ _�t�@�C���̃p�X_

+   `filename` :  
�@�@ _�t�@�C����_

+   `�߂�l` �F  
�@�@ _1(����)or0(���s)_  
�@�@ _<�P��DeleteFile�̖߂�l��Ԃ�>_

��filename���󔒕����ɂ���path�Ƀt���p�X�w��ł���

###�E�t�@�C���̑��݊m�F###
    FileExist(path, filename);

+   `path` :  
�@�@ _�t�@�C�������݂���f�B���N�g���̃p�X_

+   `filename` :  
�@�@ _�t�@�C����_

+   `�߂�l` �F  
�@�@ _1(����)or0(���s)_  
�@�@ _<�P��PathFileExists�̖߂�l��Ԃ�>_

��filename���󔒕����ɂ���path�Ƀt���p�X�w��ł���


###�E�d�����Ȃ��t�@�C�����̎擾(�t�@�C���̏㏑���h�~�p)###
    getNonFilename(path, filename);

+   `path` :  
�@�@ _�t�@�C�������݂���f�B���N�g���̃p�X_

+   `filename` :  
�@�@ _�t�@�C����_

+   `�߂�l` �F  
�@�@ _�t�@�C����������(1)��(2),(3)...�̂悤�Ɋ����t�@�C���Ɣ��Ȃ��t�@�C������Ԃ�_

###�E�f�X�N�g�b�v�̃t���p�X���擾###
    getDesktop();
+   `�߂�l` �F  
�@�@ _�f�X�N�g�b�v�̃t���p�X_  
�@�@ _<�P��SHGetSpecialFolderPath(NULL, Path, CSIDL_DESKTOP, FALSE)��Path��Ԃ�>_

###�E�f�B���N�g���I���E�B���h�E�̌Ăяo��###
    selectDir();
+   `�߂�l` �F  
�@�@�I�������f�B���N�g���̃t���p�X

