Windows��p�_�E�����[�hNPAPI for chrome 
Complex(��) ver 0.0.0.1

��ҁFPekeTaichi
�J�����Fwindows7 64bit, Visual C++ Express, Google Chrome 26.0.1403.0 dev-m
���t�F2013-02-11

�܂������Ƃ�������`�F�b�N�͏o���Ă��Ȃ��̂�
�����炭�F�X�Ɩ�肪����Ǝv���܂����c�悩������g���ĉ������B

���C���X�^���X�̐���
html
<object id="pluginId" type="application/complex" style="width: 0; height: 0;">Plugin FAILED to load</object>
js
pluginelm = document.getElementById("pluginId");
c = pluginelm.Complex();

���ȉ��A���\�b�h�̈����Ŏ󂯎��p�X�ɂ͖����Ɂu\�v��t���Ȃ��ŉ������B

���_�E�����[�h
�E�_�E�����[�h
c.download(url, savepath, filename);
    url:URL
    savepath:�ۑ���
    filename:�t�@�C����
    ��filename���󔒕����ɂ���savepath�Ƀt���p�X�w��ł���

�����̃t�@�C�����_�E�����[�h����ꍇ�͕K�����L�̂悤�ɃC���X�^���X�𕪂��Ă��������B
�_�E�����[�h�󋵂̊m�F�A�L�����Z�������o���Ȃ��Ȃ�܂��B
var c1 = pluginelm.Complex();
var c2 = pluginelm.Complex();
c1.download(url1, savepath1, filename1);
c2.download(url2, savepath2, filename2);

��L���\�b�h���s��
�E�_�E�����[�h����t�@�C���̃T�C�Y�̎擾
c.getFileSize();
    �߂�l�F�t�@�C���̃T�C�Y(�w�b�_��Content-Length:�`�̕���)
�E�_�E�����[�h�󋵂̊m�F(Bytes)
c.getNowSize();
    �߂�l�F�t�@�C���̃T�C�Y(256�o�C�g���ɍX�V)
�E�_�E�����[�h�󋵂̊m�F(��)
c.getNowPer();
    �߂�l�F�_�E�����[�h�󋵂�float�ŕԂ�(��2������������)
            �t�@�C���T�C�Y���擾�̏ꍇ��-1.0
            �_�E�����[�h�����ς݂̏ꍇ��2.0��Ԃ�

�E�_�E�����[�h�̃L�����Z���E�t�@�C���̍폜
c.setCancelFlag(1);

�E�p�r�s��
c.setFileSize(size);
c.setNowSize(size);
c.setNowPer(size);

c.getCancelFlag();
    �߂�l�F0or1(�L�����Z���ς�)

�����̑�
�E�f�B���N�g���̑��݊m�F
c.DirExist(path);
    path:�f�B���N�g���̃p�X
    �߂�l�F1(�L)or0(��)
    <�P��PathIsDirectory�̖߂�l��Ԃ�>

�E�f�B���N�g���̐���(�ċA�I)
c.makeDirs(path);
    path:�����������f�B���N�g�����܂߂��t���p�X
    �߂�l�F1(����)or0(���s)

�E�t�@�C���̍폜
c.deleteFile(path, filename);
    path:�t�@�C���̃p�X
    filename:�t�@�C����
    ��filename���󔒕����ɂ���path�Ƀt���p�X�w��ł���
    �߂�l�F1(����)or0(���s)
    <�P��DeleteFile�̖߂�l��Ԃ�>

�E�t�@�C���̑��݊m�F
c.FileExist(path, filename);
    path:�t�@�C�������݂���f�B���N�g���̃p�X
    filename:�t�@�C����
    ��filename���󔒕����ɂ���path�Ƀt���p�X�w��ł���
    �߂�l�F1(����)or0(���s)
    <�P��PathFileExists�̖߂�l��Ԃ�>

�E�d�����Ȃ��t�@�C�����̎擾
c.getNonFilename(path, filename);
    path:�t�@�C�������݂���f�B���N�g���̃p�X
    filename:�t�@�C����
    �߂�l�F�t�@�C����������(1)��(2),(3)...�̂悤�Ɋ����t�@�C���Ɣ��Ȃ��t�@�C������Ԃ�

�E�f�X�N�g�b�v�̃t���p�X���擾
c.getDesktop();
    �߂�l�F�f�X�N�g�b�v�̃t���p�X
    <�P��SHGetSpecialFolderPath(NULL, Path, CSIDL_DESKTOP, FALSE)��Path��Ԃ�>

�E�f�B���N�g���I���E�B���h�E�̌Ăяo��
c.selectDir();
    �߂�l�F�I�������f�B���N�g���̃t���p�X
