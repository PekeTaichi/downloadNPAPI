Download NPAPI Plugin for chrome, Windows
=========================================
Google Chrome�g���ׂ̈�Windows��pNPAPI�v���O�C���ł��B  
javascript����v���O�C�����Ăяo���ă_�E�����[�h�y�т��̊֘A�@�\�𗘗p�ł��܂��B  

����m�F�Fwindows7 64bit, Google Chrome 26.0.1403.0 dev-m  

���\�[�X�R�[�h�̃R���p�C��
---------------------
�\�[�X�R�[�h�̃R���p�C�����@�ɂ��Ă�[readme_src.md](https://github.com/PekeTaichi/downloadNPAPI/blob/master/readme_src.md)��ǂ�ł��������B

���X�V����
�X�V������[history.md](https://github.com/PekeTaichi/downloadNPAPI/blob/master/history.md)��ǂ�ł��������B

���g����
--------
**�`�v���O�C���̌Ăяo���`** 
###�Emanifest.json###
    "plugins" : [
        { "path": "npdownloads.dll", "public": true}
    ],

�����ۂɎg�p����ۂ́u,"public":true�v�͏�������

###�Ehtml�t�@�C��###
    <object id="pluginId" type="application/downloads" style="width: 0; height: 0;">
    Plugin FAILED to load
    </object>

###�Ejs�t�@�C��###
    pluginelm = document.getElementById("pluginId");
    dl = pluginelm.Downloads();
    dl.download(url,path,filename,0);

**�`�e���\�b�h�̐����`**

###�E�_�E�����[�h�̎��s###
    download(url, path, filename, ask);

+   `url` :  
�@�@ _�_�E�����[�h����t�@�C����URL_

+   `path` :  
�@�@ _�ۑ���̃p�X_

+   `filename` �F  
�@�@ _�ۑ�����t�@�C����_

+   `ask` �F  
�@�@ _0=�w�肵���p�X�ɕۑ�, 1=�ۑ����q�˂�_

+   `�߂�l` �F  
�@�@ _�Ȃ�_

��path�Ƀt�@�C�������܂߂��t���p�X��n���Afilename���ȗ�(�󕶎�)�ɂ��Ă�OK�ł��B  
��ask=1�̎��Apath�Efilename���w�肷�邱�ƂŖ��O��t���ĕۑ��_�C�A���O�̃f�t�H���g�p�X�A�t�@�C�������w��ł��܂��B  
  
���㏑���ۑ�����ꍇ�͊m�F�_�C�A���O���Ăяo����܂��B  
�@�u�͂��v�͏㏑���ۑ��A�u�������v�͕ʂ̖��O�ŕۑ�(���O��t���ĕۑ��_�C�A���O�Ăяo��)�A�u�L�����Z���v�̓_�E�����[�h���~�ł��B  
  
���Z�L�����e�B�΍�ׁ̈A�X�^�[�g�A�b�v�t�H���_�y�ъ��ϐ�PATH�̒ʂ����Ƃ���ɂ͕ۑ��ł��܂���B  
  
�������̃t�@�C�����_�E�����[�h����ꍇ�͕K�����L�̂悤�ɃC���X�^���X�𕪂��Ă��������B  
�_�E�����[�h�󋵂̊m�F�A�L�����Z�������o���Ȃ��Ȃ�܂��B
###�i�C���X�^���X�𕪂���j####
    var dl1 = pluginelm.Downloads();
    var dl2 = pluginelm.Downloads();
    dl1.download(url1, path1, filename1, 0);
    dl2.download(url2, path2, filename2, 0);

###�E�w�肵��URL��POST(���ăN�b�L�[���擾)###
    post(url, postmsg);

+   `url` �F  
�@�@ _POST������URL_

+   `postmsg` �F  
�@�@ _post����f�[�^(name=value&name2=value2�̌`��)_

+   `�߂�l` �F  
�@�@ _�N�b�L�[���擾�����ꍇ�N�b�L�[�̕������Ԃ��܂�_

��POST���邾����web�f�[�^�͎擾���܂���B  
�@�w�b�_�̂ݎ�M���A���̍ۂɁuSetcookie�F�`�v�̋L�q������΃N�b�L�[��ݒ肵�܂��B

###�E�N�b�L�[��ǉ����܂�(�P��)###
    addCookie(value);

+   `value` �F  
�@�@ _�ݒ肵�����N�b�L�[(name=value�̌`��)_

###�E�N�b�L�[�����������ݒ肵�����܂�(����)###
    setCookie(value);

+   `value` �F  
�@�@ _�ݒ肵�����N�b�L�[(name=value&name2=value2�̌`��)_

###�E�ݒ肳�ꂽ�N�b�L�[��S�č폜���܂�(�P��)###
    clearCookie();

###�E�ݒ肳�ꂽ�N�b�L�[���擾���܂�###
    getCookie();

+   `�߂�l` �F  
�@�@ _�ݒ肳�ꂽ�N�b�L�[�̕�����(name=value&name2=value2�̌`��)_

###�E���t�@����ݒ肵�܂�###
    setReferer(value);
+   `value` �F  
�@�@ _�ݒ肵�������t�@����URL_

###�E�ݒ肳�ꂽ�N�b�L�[���擾���܂�###
    getReferer();

+   `�߂�l` �F  
�@�@ _�ݒ肳�ꂽ���t�@����URL_

###�i��jPIXIV�Ƀ��O�C�����ĉ摜���_�E�����[�h����ꍇ###
    dl = plugin.Downloads();
    dl.post("http://www.pixiv.net/login.php", "pixiv_id=myid&pass=mypass&mode=login");
    dl.setReferer("http://www.pixiv.net/member_illust.php?mode=big&illust_id=(�摜ID)");
    dl.download(url, "", "", 1);

###�E�_�E�����[�h����t�@�C���̃T�C�Y�̎擾###
    getFileSize();

+   `�߂�l` �F  
�@�@ _�t�@�C���̃T�C�Y(�w�b�_��Content-Length :�`�̕���)_

###�E�_�E�����[�h�󋵂̊m�F(Bytes)###
    getNowSize();

+   `�߂�l` �F  
�@�@ _�t�@�C���̃T�C�Y_

###�E�_�E�����[�h�󋵂̊m�F(��)###
    getNowPer();

+   `�߂�l` �F  
�@�@ _�_�E�����[�h�󋵂�float(0�`1.0)�ŕԂ�(��2������������)_  
�@�@ _�t�@�C���T�C�Y���擾�̏ꍇ��-1.0_  
�@�@ _�_�E�����[�h�����ς݂̏ꍇ��2.0��Ԃ�_

###�E�_�E�����[�h�̃L�����Z��###
    setCancelFlag(1);

###�E�_�E�����[�h�̃G���[�̓��e�̎擾###
    getERROR();

+   `�߂�l` �F  
�@�@ _�G���[�̓��e�𕶎���Ŏ擾_  
�@�@ _"404 Not Found"�Ȃ�_  

###�E���̑��v���p�e�B�̎擾/�ݒ胁�\�b�h###
    setFileSize(value);
    setNowSize(value);
    setNowPer(value);
    getCancelFlag();
    setERROR(value);

**�`���̑��̃��\�b�h�̐����`**

###�E�f�B���N�g���̑��݊m�F###
    dirExist(path);

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

###�E�t�@�C���̑��݊m�F###
    fileExist(path, filename);

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

###�E�f�B���N�g���I���_�C�A���O�̌Ăяo��###
    selectDirDialog();

+   `�߂�l` �F  
�@�@�I�������f�B���N�g���̃t���p�X

###�E���O��t���ĕۑ��_�C�A���O�̌Ăяo��###
    saveFileDialog(path, filename);

+   `path` :  
�@�@ _�f�t�H���g�̃p�X(�󕶎���)_

+   `filename` :  
�@�@ _�f�t�H���g�̃t�@�C����(�󕶎���)_

+   `�߂�l` �F  
�@�@�w�肵���t���p�X

###�E�x���_�C�A���O�̌Ăяo��###
    alertDialog(msg);

+   `msg` :  
�@�@ _�x����_

###�E�m�F�_�C�A���O�̌Ăяo��###
    askDialog(msg, title, flag);

+   `msg` :  
�@�@ _�m�F���b�Z�[�W_

+   `filename` :  
�@�@ _�_�C�A���O�̃^�C�g��(�^�C�g���o�[�̕���)_

+   `flag` :  
�@�@ _1=�͂��E�������E�L�����Z��, 0=�͂��E������_

+   `�߂�l` �F  
�@�@_1=�͂�, 0=������, -1=�L�����Z��_

###�E�w�肵���p�X���G�N�X�v���[���[�ŊJ��###
    openExplorer(path);

+   `path` :  
�@�@ _�J�������p�X_

