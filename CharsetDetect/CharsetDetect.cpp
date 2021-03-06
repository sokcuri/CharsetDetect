// CharsetDetect.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../compact_enc_det/compact_enc_det.h"

using tupler = std::tuple<std::string, int>;
int main()
{
	std::vector<std::tuple<std::string, int>> testList = {
		{ "big5-utf8.txt", Encoding::UTF8 },
		{ "big5.txt", Encoding::CHINESE_BIG5 },
		{ "big5hkscs-utf8.txt", Encoding::UTF8 },
		{ "big5hkscs.txt", Encoding::CHINESE_BIG5_CP950 },
		{ "cp949-utf8.txt", Encoding::UTF8 },
		{ "cp949.txt", Encoding::KOREAN_EUC_KR },
		{ "euc_jisx0213-utf8.txt", Encoding::UTF8 },
		{ "euc_jisx0213.txt", Encoding::JAPANESE_EUC_JP },
		{ "euc_jp-utf8.txt", Encoding::UTF8 },
		{ "euc_jp.txt", Encoding::JAPANESE_EUC_JP },
		{ "euc_kr-utf8.txt", Encoding::UTF8 },
		{ "euc_kr.txt", Encoding::KOREAN_EUC_KR },
		{ "gb18030-utf8.txt", Encoding::UTF8 },
		{ "gb18030.txt", Encoding::GB18030 },
		{ "gb2312-utf8.txt", Encoding::UTF8 },
		{ "gb2312.txt", Encoding::CHINESE_GB },
		{ "gbk-utf8.txt", Encoding::UTF8 },
		{ "gbk.txt", Encoding::CHINESE_GB },
		{ "hz-utf8.txt", Encoding::UTF8 },
		{ "hz.txt", Encoding::HZ_GB_2312 },
		{ "shift_jis-utf8.txt", Encoding::UTF8 },
		{ "shift_jis.txt", Encoding::JAPANESE_SHIFT_JIS },
		{ "shift_jisx0213-utf8.txt", Encoding::UTF8 },
		{ "shift_jisx0213.txt", Encoding::JAPANESE_SHIFT_JIS },
	};

	char lpszFileName[MAX_PATH];
	GetModuleFileNameA(NULL, lpszFileName, MAX_PATH);
	for (int i = strlen(lpszFileName) - 1; i >= 0; i--)
	{
		if (lpszFileName[i] == '\\')
		{
			lpszFileName[i + 1] = 0;
			strcat(lpszFileName, "..\\..\\test\\");
			break;
		}
	}

	for (const auto &t : testList)
	{
		FILE *fp = fopen(std::string(lpszFileName + std::get<0>(t)).c_str(), "r");
		if (!fp)
		{
			printf(std::string("File not open: " + std::get<0>(t) + "\n").c_str());
			return false;
		}

		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char *text = (char *)malloc(fsize + 1);
		fread(text, fsize, 1, fp);

		fclose(fp);
		text[fsize] = 0;
		
		bool is_reliable;
		int bytes_consumed;
		Encoding encoding = CompactEncDet::DetectEncoding(
			text, strlen(text),
			nullptr, nullptr, nullptr,
			UNKNOWN_ENCODING,
			Language::KOREAN,
			CompactEncDet::QUERY_CORPUS,
			false,
			&bytes_consumed,
			&is_reliable);

		printf(std::string(std::get<0>(t) + ": ").c_str());
		printf((encoding == std::get<1>(t)) ? "Success\n" : std::string("Fail (Expected: " + std::to_string(std::get<1>(t)) + ", Real: " + std::to_string(encoding) + ")\n").c_str());
		fclose(fp);
	}
    return 0;
}

