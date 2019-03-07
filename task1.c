#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int maxClass,maxCount,a[10000][200],b[5000][200],i,j,jj,k,l,m,r,temp,temp1,num_instances=6670,t_data=3333,blockSize,features=192,e[4][26];

//struct for maintaining class and distance of a point from a reference point
typedef struct disttAndClass{
	float distt;
	int class;
}disttAndClass;
disttAndClass d[4600],t[7000];
//comparator for sorting them to gain first k neighbours
int custom(const void *p, const void *q){
	float l=((disttAndClass *)p)->distt;
	float r=((disttAndClass *)q)->distt;
	return (l-r)>0;
}

int main(){
	FILE *f=fopen("pp_tra.dat","r");
	FILE *f1=fopen("pp_tes.dat","r");
	blockSize=num_instances/3;
	for(i=0;i<num_instances;i++){
		for(j=0;j<features+1;j++){
			fscanf(f,"%d",&temp);
			a[i][j]=temp;
		}
	}
	for(r=0;r<t_data;r++){
		for(m=0;m<features+1;m++){
			fscanf(f1,"%d",&temp1);
			b[r][m]=temp1;
		}
	}
	fclose(f);
	fclose(f1);
		
	//initializing error to 0
	for(i=1;i<=3;i++)
		for(j=1;j<=25;j++)
			e[i][j]=0;
	//when block1 is used as validation set,(block2+block3 as training set)
	for(i=0;i<blockSize;i++){
		for(j=blockSize;j<num_instances;j++){
			d[j-blockSize].distt=0;
			//calculating Euclidian distance from one point in validation to all others in other blocks
			for(l=0;l<features;l++)
				d[j-blockSize].distt+=(a[i][l]-a[j][l])*(a[i][l]-a[j][l]);
			d[j-blockSize].distt=sqrt(d[j-blockSize].distt);
			d[j-blockSize].class=a[j][192];
		}
		//sorting them
		qsort(d,2*blockSize+1,sizeof(disttAndClass),custom);
		//finding for first k neighbours
		for(k=1;k<=25;k++){
			int count[10]={0};
			for(j=0;j<k;j++)
				count[d[j].class]++;
			maxClass=0;
			maxCount=count[0];
			//finding the max probable class
			for(j=0;j<10;j++)
				if(count[j]>=maxCount){
					maxClass=j;
					maxCount=count[j];
				}
			//incrementing error count if not equal
			 if(maxClass!=a[i][192])
			 	e[1][k]++;
		}
	}

	//when block2 is used as validation set,(block1+block3 as training set)
	for(i=blockSize;i<2*blockSize;i++){
		for(j=jj=0;j<num_instances;j++)
			if(j<blockSize || j>=2*blockSize){
				d[jj].distt=0;
				for(l=0;l<features;l++)
					d[jj].distt+=(a[i][l]-a[j][l])*(a[i][l]-a[j][l]);
				d[jj].distt=sqrt(d[jj].distt);
				d[jj].class=a[j][192];
				jj++;
		}
		qsort(d,2*blockSize+1,sizeof(disttAndClass),custom);
		for(k=1;k<=25;k++){
			int count[10]={0};
			for(j=0;j<k;j++)
				count[d[j].class]++;
			maxClass=0;
			maxCount=count[0];
			for(j=0;j<10;j++)
				if(count[j]>=maxCount){
					maxClass=j;
					maxCount=count[j];
				}
			 if(maxClass!=a[i][192])
			 	e[2][k]++;
		}
	}
	
	//when block3 is used as validation set,(block1+block2 as training set)
	for(i=2*blockSize;i<num_instances;i++){
		for(j=0;j<2*blockSize;j++){
			d[j].distt=0;
			for(l=0;l<features;l++)
				d[j].distt+=(a[i][l]-a[j][l])*(a[i][l]-a[j][l]);
			d[j].distt=sqrt(d[j].distt);
			d[j].class=a[j][192];
		}
		qsort(d,2*blockSize,sizeof(disttAndClass),custom);
		for(k=1;k<=25;k++){
			int count[10]={0};
			for(j=0;j<k;j++)
				count[d[j].class]++;
			maxClass=0;
			maxCount=count[0];
			for(j=0;j<10;j++)
				if(count[j]>=maxCount){
					maxClass=j;
					maxCount=count[j];
				}
			 if(maxClass!=a[i][192])
			 	e[3][k]++;
		}
	}
	for(i=1;i<=3;i++){
		printf("Error for fold %d: ", i);
		for(j=1;j<=25;j++){
			printf("%d ",e[i][j]);
		}
		printf("\n");
	}
	float sum[26];
	//finding average error for each k
	printf("Average Error: ");
	for(k=1;k<=25;k++){
		sum[k]=0;
		for(i=1;i<=3;i++){
			sum[k]+=e[i][k];
		}
		sum[k]=sum[k]/3;
		printf("%g ",sum[k]);
	}
	//finding k having minimum error
	float mini=sum[1];
	int minIndex=1;
	for(k=2;k<=25;k++)
		if(mini>=sum[k]){
			mini=sum[k];
			minIndex=k;}
	printf("\nK Value = %d Min_Value = %g\n",minIndex,mini);

	int e1=0;
	//applying similar procedure for testing by considering the obtained k
	for(r=0;r<t_data;r++){
		for(i=0;i<num_instances;i++){
			t[i].distt=0;
			for(j=0;j<features;j++)
				t[i].distt+=(b[r][j]-a[i][j])*(b[r][j]-a[i][j]);
					t[i].distt=sqrt(t[i].distt);
					t[i].class=a[i][192];
				}	qsort(t,num_instances,sizeof(disttAndClass),custom);
	int cnt[10]={0};
			for(j=0;j<minIndex;j++)
				cnt[t[j].class]++;
			int mc=0;
			int mcnt=cnt[0];
			for(j=0;j<10;j++)
				if(cnt[j]>=mcnt){
					mc=j;
					mcnt=cnt[j];
				}
			 if(mc!=b[r][192])
			 	e1++;
		}
	printf("Wrongly Classified: %d\nRightly Classified: %d\n",e1,t_data-e1);
	int correct=t_data-e1;
	float accuracy=100.0*((1.0)*correct)/(1.0*t_data);
	printf("Accuracy = %f %%\n",accuracy);
	return 0;
}