#ifndef __AFXWAPI__
#define __AFXWAPI__
/**
 * [�ύX����]
 * #001 256��MAXPATH
 * #002 delete dummy
 * #003 windowId BYTE��LONG
 * #004 szWildName�폜 (����w���ł���Ă��炤)
 * #005 �������΃f�B���N�g���ƃ^�C�g���w���ǉ�
 * #006 ���ł̂��ߏȗ�
 * #007 ApiInternalCopy()��ǉ�
 * #008 ApiInternalCopy -> ApiIntCopyTo
 */

#include "windows.h"

#define API_MAX_PLUGIN_NAME_LENGTH	63
#define API_MAX_AUTHOR_NAME_LENGTH	31
#define API_MAX_PATH_LENGTH			MAX_PATH // #001 256��MAXPATH
#define AFXW_WINDOW_ID_SRC			0
#define AFXW_WINDOW_ID_DST			1

#define MAKE_VERSION(major, minor, revision, build)	\
		( (major    & 0xFF) << 24) | ( (minor & 0xFF) << 16) | \
		( (revision & 0xFF) << 8 ) | ( (build & 0xFF)      )

#define APISUCCESS(ret)				(ret == 1)
#define APIFAILE(ret)				(ret == 0)

#pragma pack(push, 1)

typedef HGLOBAL HAFX;

/**
 * ����w������n�������
 */
typedef struct {
	HWND		hWnd;		// ����w�̃E�B���h�E�n���h��
	DWORD		windowId;	// AFXW_WINDOW_ID_* #003
	LPVOID		Reserv[8];	// #006
} AfxwInfo, *lpAfxwInfo;

/**
 * �v���O�C�����
 */
typedef struct {
	WCHAR		szPluginName[API_MAX_PLUGIN_NAME_LENGTH + 1];
	WCHAR		szAuthorName[API_MAX_AUTHOR_NAME_LENGTH + 1];
	DWORD		dwVersion;
	DWORD		dwReserv1;
	DWORD		dwReserv2;
	DWORD		dwReserv3;
	DWORD		dwReserv4;
} ApiInfo, *lpApiInfo;

/**
 * ApiOpen�ŕԂ����
 * #005 ApiFindFirst()��szDirPath�ɂ́A�u��f�B���N�g���v�{�u�������΃f�B���N�g���v��
 * #005 �n�����B�u�������΃f�B���N�g���v���u���[�g�i/)�v����ɍs���ƃv���O�C���𔲂���B
 */
typedef struct {
	WCHAR		szBaseDir[MAX_PATH];    // ��p�X
	WCHAR		szInitRelDir[MAX_PATH]; // ���΃p�X #005
	WCHAR		szTitle[64];            // �^�C�g�� #005
	DWORD		dwReserv1;
	DWORD		dwReserv2;
	DWORD		dwReserv3;
	DWORD		dwReserv4;
} ApiOpenInfo, *lpApiOpenInfo;

/**
 * �A�C�e�����
 */
typedef struct {
	WCHAR		szItemName[API_MAX_PATH_LENGTH];
	ULONGLONG	ullItemSize;
	FILETIME 	ullTimestamp;
	DWORD		dwAttr;			// FindFirstFile��GetFileAttributes�Ɠ��l(FILE_ATTRIBUTE_*)
	DWORD		dwReserv1;
	DWORD		dwReserv2;
	DWORD		dwReserv3;
	DWORD		dwReserv4;
} ApiItemInfo, *lpApiItemInfo;

/**
 * �R�s�[�E�ړ��̕���
 */
typedef enum {
	ApiDirection_A2P,		// ����w����v���O�C����
	ApiDirection_P2A,		// �v���O�C�����炠��w��
} ApiDirection;

/**
 * �v���O�C�������擾����B
 * @param[out] pluginInfo    �v���O�C�����B
 */
void WINAPI ApiGetPluginInfo(lpApiInfo pluginInfo);

/**
 * �v���O�C���I�[�v������B
 * @param[in]  szCommandLine �v���O�C�����ɓn���R�}���h���C���B
 * @param[in]  afxwInfo      ����w�̏��B
 * @param[out] openInfo      �I�[�v�����B
 * @retval     0�ȊO         �v���O�C���Ŏ��R�Ɏg����n���h���B���̑�API��handle�Ƃ��Ďg�p����B
 * @retval     NULL(0)       �I�[�v�����s�B
 */
HAFX WINAPI ApiOpen(LPCWSTR szCommandLine, const lpAfxwInfo afxwInfo, lpApiOpenInfo openInfo);

/**
 * �v���O�C���N���[�Y����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiClose(HAFX handle);

/**
 * szDirPath�Ŏw�肳�ꂽ���z�f�B���N�g���p�X�̐擪�A�C�e�����擾����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szDirPath     �擾�������f�B���N�g���t���p�X�B
 * @param[in]  szWildName    �A�C�e���̃��C���h�J�[�h�w��B
 * @param[out] lpItemInfo    �擪�A�C�e���̏��B
 * @retval     1             �A�C�e�����������ꍇ
 * @retval     0             �G���[
 * @retval     -1            �����I��
 */
int  WINAPI ApiFindFirst(HAFX handle, LPCWSTR szDirPath, /*LPCWSTR szWildName, #004 delete */ lpApiItemInfo lpItemInfo);

/**
 * ApiFindFirst�Ŏ擾�����A�C�e���ȍ~�̃A�C�e�����擾����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[out] lpItemInfo    �擪�A�C�e���̏��B
 * @retval     1             �A�C�e�����������ꍇ
 * @retval     0             �G���[
 * @retval     -1            �����I��
 */
int  WINAPI ApiFindNext(HAFX handle, lpApiItemInfo lpItemInfo);

/**
 * �v���O�C�����炠��w���ɃA�C�e�����R�s�[����B
 * ����w�ŃR�s�[����������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szFromItem    �R�s�[���̃A�C�e�����B
 * @param[in]  szToPath      �R�s�[��̃t�H���_�B
 * @param[in]  lpPrgRoutine  �R�[���o�b�N�֐�(CopyFileEx�Ɠ��l)
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiCopyTo(HAFX handle, LPCWSTR szFromItem, LPCWSTR szToPath, LPPROGRESS_ROUTINE lpPrgRoutine);

/**
 * ����w�������I�ɗ��p����R�s�[�����B
 * ��Ƀv���O�C�����̃t�@�C�����J���ꍇ�ɁAAFXWTMP�ȉ��ɃR�s�[���邽�߂Ɏg����B
 * �������A���ۂɃR�s�[���邩�ǂ����̓v���O�C������ŁA����w��szOutputPath�Ŏw�肳�ꂽ�t�@�C�����J���B
 *
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szFromItem    �v���O�C�����̃A�C�e����
 * @param[in]  szToPath      �R�s�[��̃t�H���_(c:\Program Files\AFXWTMP.0\hoge.txt�����w�肳���)
 * @param[out] szOutputPath  �v���O�C�������ۂɃR�s�[�����t�@�C���p�X
 *                           szToPath�ɃR�s�[����ꍇ��szToPath��szOutputPath�ɃR�s�[���Ă���w�ɕԂ��B
 *                           szToPath�ɃR�s�[���Ȃ��ꍇ�A����w�ɊJ���Ăق����t�@�C���̃p�X���w�肷��B
 * @param[in]  dwOutPathSize  szOutputPath�̃o�b�t�@�T�C�Y
 * @param[in]  lpPrgRoutine  �R�[���o�b�N�֐�(CopyFileEx�Ɠ��l)
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiIntCopyTo(HAFX handle, LPCWSTR szFromItem, LPCWSTR szToPath, LPWSTR szOutputPath, DWORD dwOutPathSize, LPPROGRESS_ROUTINE lpPrgRoutine);

/**
 * �A�C�e�����폜����B
 * ����w�ō폜������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szItemPath    �폜����A�C�e���̃t���p�X�B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiDelete(HAFX handle, LPCWSTR szItemPath);

/**
* �A�C�e�����g���q���ʎ��s����B
* ����w��ENTER���������Ƃ��ɌĂяo�����B
* @param[in]  handle        ApiOpen�ŊJ�����n���h���B
* @param[in]  szItemPath    �A�C�e���̃t���p�X�B
* @retval     2             ����w���ɏ�����C����B�iApiCopy�Ńe���|�����ɃR�s�[���Ă�����s)
* @retval     1             ����
* @retval     0             �G���[
*/
int  WINAPI ApiExecute(HAFX handle, LPCWSTR szItemPath);

/**
* �A�C�e�����g���q���ʎ��s����B
* ����w��SHIFT-ENTER���������Ƃ��ɌĂяo�����B
* @param[in]  handle        ApiOpen�ŊJ�����n���h���B
* @param[in]  szItemPath    �A�C�e���̃t���p�X�B
* @retval     2             ����w���ɏ�����C����B�iApiCopy�Ńe���|�����ɃR�s�[���Ă�����s)
* @retval     1             ����
* @retval     0             �G���[
*/
int  WINAPI ApiExecute2(HAFX handle, LPCWSTR szItemPath);


//------------------------------ �ȉ��|�X�g���[���` --------------------------------

/**
 * ����w������v���O�C���ɃA�C�e�����R�s�[����B
 * ����w�ŃR�s�[����������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szFromItem    �R�s�[���̃A�C�e�����B
 * @param[in]  szToPath      �R�s�[��̃t�H���_�B
 * @param[in]  lpPrgRoutine  �R�[���o�b�N�֐�(CopyFileEx�Ɠ��l)
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiCopyFrom(HAFX handle, LPCWSTR szFromItem, LPCWSTR szToPath, LPPROGRESS_ROUTINE lpPrgRoutine);

/**
 * �A�C�e�����ړ�����B
 * ����w�ňړ�����������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szFromPath    �ړ����̃t���p�X�B
 * @param[in]  szToPath      �ړ���̃t���p�X�B
 * @param[in]  direction     ApiDirection_A2P�͂���w����v���O�C�����Ɉړ�����ꍇ�B
 *                           ApiDirection_P2A�̓v���O�C�������炠��w�Ɉړ�����ꍇ�B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiMove(HAFX handle, LPCWSTR szFromPath, LPCWSTR szToPath, ApiDirection direction);

/**
 * �A�C�e�������l�[������B
 * ����w�Ń��l�[��������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szFromPath    ���l�[���O�̃t���p�X�B
 * @param[in]  szToName      ���l�[����̃t�@�C�����B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiRename(HAFX handle, LPCWSTR szFromPath, LPCWSTR szToName);

/**
 * �f�B���N�g�����쐬����B
 * ����w�Ńf�B���N�g�����쐬������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szItemPath    �쐬����f�B���N�g���̃t���p�X�B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiCreateDirectory(HAFX handle, LPCWSTR szItemPath);

/**
 * �f�B���N�g�����폜����B
 * ����w�Ńf�B���N�g�����폜������Ƃ��ɌĂяo�����B
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  szItemPath    �폜����f�B���N�g���̃t���p�X�B
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiRemoveDirectory(HAFX handle, LPCWSTR szItemPath);

/**
 * �R���e�L�X�g���j���[�\��
 * @param[in]  handle        ApiOpen�ŊJ�����n���h���B
 * @param[in]  hWnd          ����w�̃E�B���h�E�n���h���B
 * @param[in]  x             ���j���[�\��X���W�B
 * @param[in]  y             ���j���[�\��Y���W�B
 * @param[in]  szItemPath    �A�C�e���̃t���p�X�B
 * @retval     2             ����w���ɏ�����C����B�iApiCopy�Ńe���|�����ɃR�s�[���Ă���R���e�L�X�g���j���[?)
 * @retval     1             ����
 * @retval     0             �G���[
 */
int  WINAPI ApiShowContextMenu(HAFX handle, const HWND hWnd, DWORD x, DWORD y, LPCWSTR szItemPath);

#pragma pack(pop)
#endif	/* __AFXWAPI__ */

