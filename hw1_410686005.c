#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define PI 3.14159265358979323846

int main(void)
{
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

    char *filename[8]={
        "wave_0Hz.wav","wave_110Hz.wav","wave_220Hz.wav","wave_440Hz.wav",
        "wave_880Hz.wav","wave_1760Hz.wav","wave_3520Hz.wav","wave_7040Hz.wav"
    };
    int k[8]={0,1,2,4,8,16,32,64};

    int time=1;
    int NumSamples=time*SampleRate;
    Subchunk2Size=NumSamples * NumChannels * BitsPerSample/8;
    ChunkSize=NumSamples * NumChannels * BitsPerSample/8+36;

    FILE *file;
    short data[Subchunk2Size];
    int w;

    for(int i=0;i<8;i++)
    {
        if((file=fopen(filename[i],"wb"))!=NULL)
        {
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
            fclose(file);


        }
        else
        {
            printf("GG!");
        }
    }
   
}