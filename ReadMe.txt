========================================================================
       MICROSOFT FOUNDATION CLASS ���C�u���� : RayTrace
========================================================================


AppWizard ���������� RayTrace �ɂ́AMicrosoft Foundation Class �̊�{�I��
�g������������Ă��܂��B�A�v���P�[�V�����쐬�̂ЂȌ^�Ƃ��Ă��g�����������B

���̃t�@�C���ɂ� RayTrace ���\�����Ă���e�t�@�C���̊T�v�������܂܂�Ă��܂��B

RayTrace.h
    ���̃t�@�C���̓A�v���P�[�V�����̒��S�ƂȂ�C���N���[�h�t�@�C���ł��B����
    �t�@�C���͑��̃v���W�F�N�g�̌ŗL�̃C���N���[�h�t�@�C���iResource.h  ����
    �݂܂��j ���C���N���[�h���A�܂� CRayTraceApp �A�v���P�[�V�����N���X���
    �����܂��B

RayTrace.cpp
    ���̃t�@�C���� CRayTraceApp �A�v���P�[�V�����N���X���܂ރA�v���P�[�V����
    �̒��S�ƂȂ�\�[�X�t�@�C���ł��B

RayTrace.rc
    ���̃t�@�C���̓v���O�������g�p���� Microsoft Windows �̃��\�[�X��񋓂���
    ���B���̃t�@�C���� RES �T�u�f�B���N�g���ɕۑ�����Ă���A�C�R���A�r�b�g�}
    �b�v�A�J�[�\�����܂݂܂��B���̃t�@�C���� Microsoft Developer Studio �Œ���
    �ҏW�\�ł��B

res\RayTrace.ico
    ���̃t�@�C���́A�A�v���P�[�V�����̃A�C�R���Ƃ��Ďg�p�����A�C�R���t�@�C
    ���ł��B���̃A�C�R���̓��\�[�X �t�@�C�� RayTrace.rc �ɂ���ăC���N���[�h
    ����܂��B

res\RayTrace.rc2
    ���̃t�@�C���� Microsoft Developer �ɂ���ĕҏW����Ȃ����\�[�X���܂�ł���
    ���B���̃t�@�C���Ƀ��\�[�X �G�f�B�^�ŕҏW����Ȃ����ׂẴ��\�[�X���L�q
    ���Ă��������B

RayTrace.reg
    ���̃��W�X�g�� �t�@�C���̓t���[�����[�N�̐ݒ�@���Љ�邽�߂̃T���v��
    �t�@�C���ł��B
    �A�v���P�[�V�����ƈꏏ�Ɏg�p���邩�A�܂��͍폜���ăf�t�H���g�̓o�^ 
     RegisterShellFileTypes ���g�p���܂��B

RayTrace.clw
    ���̃t�@�C���� ClassWizard  �������̃N���X�̕ҏW��N���X��V�K�쐬�����
    ���Ɏg�����������Ă��܂��B�܂����̃t�@�C���́AClassWizard �ɂ�郁�b�Z
    �[�W �}�b�v��_�C�A���O �f�[�^ �}�b�v�̕ҏW�␶���A�v���g�^�C�v �����o��
    ���̐����ɕK�v�ȏ��������Ă��܂��B

/////////////////////////////////////////////////////////////////////////////

���C�� �t���[�� �E�B���h�E:

MainFrm.h, MainFrm.cpp
    �����̃t�@�C���̓t���[�� �N���X CMainFrame ���܂݂܂��B
    �t���[�� �N���X CMainFrame �� CMDIFrameWnd �N���X����h�����AMDI ��
    �t���[���𐧌䂵�܂��B

res\Toolbar.bmp
    ���̃r�b�g�}�b�v �t�@�C���̓c�[���o�[�̃C���[�W�Ƃ��Ďg���܂��B �c�[���o
    �[��X�e�[�^�X�o�[�̏����ݒ�� CMainFrame �N���X�ɂ���č\�z�����
    ���B�c�[���o�[�̃{�^���R���g���[����ǉ����������ɂ� MainFrm.cpp 
    �t�@�C�����̔z��ɂ��������ăc�[���o�[�̃r�b�g�}�b�v��ҏW���Ă��������B

/////////////////////////////////////////////////////////////////////////////

AppWizard �� 1 �̃h�L�������g�^�C�v�� 1 �̃r���[���쐬���܂��B

RayTraceDoc.h, RayTraceDoc.cpp - �h�L�������g
    �����̃t�@�C���� CRayTraceDoc �N���X���܂݂܂��B���ʂȃh�L�������g �f
    �[�^��t����������ACRayTraceDoc::Serialize ���g�����t�@�C���̃Z�[�u��
    ���[�h�@�\���C���v�������g�������Ƃ��͂��̃t�@�C����ҏW���Ă��������B

RayTraceView.h, RayTraceView.cpp - �r���[
    �����̃t�@�C���� CRayTraceView �N���X���܂݂܂��B
    CRayTraceView �I�u�W�F�N�g�� CRayTraceDoc �I�u�W�F�N�g��\�����邽�߂�
    �g���܂��B

res\RayTraceDoc.ico
    ���̃t�@�C���� CRayTraceDoc �N���X�� MDI �q�E�B���h�E�p�A�C�R���Ƃ��Ďg
    ����A�C�R���t�@�C���ł��B���̃A�C�R���̓��\�[�X�t�@�C�� RayTrace.rc
    �ɂ���ăC���N���[�h����܂��B


/////////////////////////////////////////////////////////////////////////////
���̑��̕W���t�@�C��:

StdAfx.h, StdAfx.cpp
    �����̃t�@�C���̓v���R���p�C���σw�b�_�[ �t�@�C�� (PCH) RayTrace.pch 
    ��v���R���p�C���ό^�t�@�C�� StdAfx.obj ���\�z���邽�߂Ɏg����t�@�C���ł��B

Resource.h
    ���̃t�@�C���͐V�K���\�[�X ID ���`����W���w�b�_�[ �t�@�C���ł��B
    Microsoft Developer Studio �͂��̃t�@�C����ǂݍ��ݍX�V���܂��B

/////////////////////////////////////////////////////////////////////////////
���̑��̒���:

"TODO:" �Ŏn�܂�R�����g�́A�\�[�X�R�[�h�̒ǉ���J�X�^�}�C�Y�̕K�v�ȏꏊ����
���܂��B

�A�v���P�[�V�����ŋ��L DLL ���� MFC ���g�p����ꍇ��A�A�v���P�[�V����
���I�y���[�e�B���O �V�X�e���Ō��ݎg�p���Ă��錾��ȊO�̌�����g�p���Ă�
��ꍇ�́AMicrosoft Visual C++ CD-ROM �� system �܂��� system32 �f�B���N�g
������A���[�J���C�Y���ꂽ���\�[�X�ɑΉ����� MFC40XX.DLL ���R�s�[���āA
�t�@�C������ MFCLOC.DLL �ɂ���K�v������܂��B("XXX" �͌���̏ȗ��`����
���܂��B�Ⴆ�΁A MFC40DEU.DLL �̓h�C�c��ɖ|�󂳂ꂽ���\�[�X���܂݂܂��B)
���̂悤�ɂ������Ȃ��ꍇ�́A�A�v���P�[�V�������̂������� UI �v�f�̓I�y
���[�e�B���O �V�X�e���̌���̂܂܎c��܂��B

/////////////////////////////////////////////////////////////////////////////
