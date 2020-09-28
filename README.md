# DSP Homework-1
<small>410686005 通訊四 李冠達<p align="right">2020.9.27</p></small>
---

首先因為需要用到$\sin(.)$與 $\mbox{floor}(.)$ 等函數，因此需要匯入<math.h>檔，如下程式碼：
```C
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
```
由於需要將sin弦波寫入wav.檔中，因此需要將wav header寫入檔案開頭，格式如下：
|Offset|  Size | Name   |          Description|
| --------:   | -----:  | ----  |-----------|
|0       |4   |ChunkID  |        Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).|
|4       |4   |ChunkSize|36 + SubChunk2Size, or more precisely: 4 + ( 8 + SubChunk1Size ) + ( 8 + SubChunk2Size ) 
|8       |4   |Format  |         Contains the letters "WAVE"(0x57415645 big-endian form).|
|12      |4   |Subchunk1ID |Contains the letters "fmt "(0x666d7420 big-endian form).|
|16      |4   |Subchunk1Size  |16 for PCM.  This is the size of the rest of the Subchunk which follows this number.|
|20      |2   |AudioFormat  |PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.|
|22      |2   |NumChannels  | Mono = 1, Stereo = 2, etc.|
|24      |4   |SampleRate   | 8000, 44100, etc.|
|28      |4   |ByteRate     | == SampleRate * NumChannels * BitsPerSample/8|
|32      |2   |BlockAlign   | == NumChannels * BitsPerSample/8 |
|34      |2   |BitsPerSample |   8 bits = 8, 16 bits = 16, etc.|
|36      |4   |Subchunk2ID    |  Contains the letters "data" (0x64617461 big-endian form).|
|40      |4   |Subchunk2Size  |  == NumSamples * NumChannels * BitsPerSample/8|
|44       | *   |Data         |    The actual sound data.|

因此透過上表資料，我們可以先做以下宣告：
```C
    char ChunkID[5]="RIFF";
    int ChunkSize;
    char Format[5]="WAVE";
    char Subchunk1ID[5]="fmt ";
    int Subchunk1Size=16;
    short AudioFormat=1;
    short NumChannels=1;
    int SampleRate=16000; 
    short BitsPerSample=16;
    int ByteRate=SampleRate*NumChannels*BitsPerSample/8;
    short BlockAlign=NumChannels * BitsPerSample/8;
    char Subchunk2ID[5]="data";
    int Subchunk2Size;
```
再來將輸出檔名的字串設定到一個指標陣列中，共8個檔案，分別為110的二的次方倍；與k的設定值分別存入陣列中：
```C
    char *filename[8]={
        "wave_0Hz.wav","wave_110Hz.wav","wave_220Hz.wav","wave_440Hz.wav",
        "wave_880Hz.wav","wave_1760Hz.wav","wave_3520Hz.wav","wave_7040Hz.wav"
    };
    int k[8]={0,1,2,4,8,16,32,64};
```
再來設定檔案的長度，在這邊我們將它設定成1秒鐘，因為Sample rate為每秒取樣次數，因此反過來說，每間隔$\frac{1}{Sample Rate}$會取樣一次，因為總時間除以取樣間的間隔時間可以得到資料的總數。
<center>$ NumSamples=\frac{Time}{\frac{1}{Sample Rate}}=Time*SampleRate $</center>

有了這個變數，我們就可以找到時間與點數的關係，並將檔案大小藉由前面提到的wav header找到關係。程式碼如下：
```C
    int time=1;
    int NumSamples=time*SampleRate;
    Subchunk2Size=NumSamples * NumChannels * BitsPerSample/8;
    ChunkSize=NumSamples * NumChannels * BitsPerSample/8+36;
```

再來就是將動作利用for迴圈執行8次一樣的動作(因為有8個檔案),若檔案順利開啟，即開始產生弦波檔案；若失敗，則會看到 "GG!" 字樣。
```C
    for(int i=0;i<8;i++)
    {
        if((file=fopen(filename[i],"wb"))!=NULL)
        {
            .
            .
            .
        }
        else
        {
            printf("GG!");
        }
    }
```
檔案順利開啟時，先將wav header寫入檔案中：
```
            fwrite(ChunkID,4,sizeof(char),file);
	        fwrite(&ChunkSize,1,sizeof(int),file);
	        fwrite(Format,4,sizeof(char),file);
   		    fwrite(Subchunk1ID,4,sizeof(char),file);
   		    fwrite(&Subchunk1Size,1,sizeof(int),file);
		    fwrite(&AudioFormat,1,sizeof(short),file);
 		    fwrite(&NumChannels,1,sizeof(short),file);
		    fwrite(&SampleRate,1,sizeof(int),file);
	        fwrite(&ByteRate,1,sizeof(int),file);
	        fwrite(&BlockAlign,1,sizeof(short),file);
	        fwrite(&BitsPerSample,1,sizeof(short),file);
	        fwrite(Subchunk2ID,4,sizeof(char),file);
		    fwrite(&Subchunk2Size,1,sizeof(int),file);
```
由於
<center>$w(t)=
\begin{cases}
1, & \mbox{if }0\leq t<0.5 \\
0, & \mbox{if }0.5\leq t<1\\
\end{cases}$</center>

是題目給定條件，因此這裡使用$\mbox{if-else}$將兩種情況分開，並帶入
<center>$x(t)=\sin(2\pi kft)w(t)$</center>

由於是離散時間訊號，因此在$\sin(.)$的括號內除以Sample rate，最後將整個函數正規化即完成此程式，最後步驟如下：
```C
            for(int j=0;j<NumSamples;j++)
            {
                if(j>=(0.5*SampleRate))
                {
                    w=0;
                }
                else
                {
                    w=1;
                }
                data[j]=floor(0.1*32367*sin(2*PI*j*110*k[i]/SampleRate)+0.5)*w;
                fwrite(&data[j],1,sizeof(short),file);
            }
```
