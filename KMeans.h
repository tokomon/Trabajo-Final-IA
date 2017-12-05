/*Universidad Católica San Pablo
**Integrantes:
+++++++++++++++
**~Wilber Cutire
**~Yessica Chuctaya
**~Angie Valeriano
**~Ximena Pocco
+++++++++++++++
#Basado en https://github.com/drobison/k-means
***/

class Cluster
{
	int dataPointDim;

	int numberOfClusters;

	bool foundDataPointDim;

	vector<vector<float> > inputData;

	vector<vector<float> > centroids;

	vector<int> assignedClusters;

	void initCentroids();

	float getEuclideanDistance(vector<float> sample, vector<float> centroid);

	void assignClusters();

	void updateCentroids();

	~Cluster();

public:
	Cluster();

	void process(string file);

	void setNumberOfClusters(int numberOfClusters);
	
	int getNumberOfClusters();
	int prediccion_basada_en_Distancia(int User,int Item);
	float prediccionCompletaComp(string file, string original);
	void prediccionCompleta(string file);//, string original);
	void Imprimir(string file);
};

map<int,map<int,int> > M;
map<string,pair<int,int> > Books;

Cluster::Cluster()
{
	dataPointDim = 0;
	numberOfClusters = 0;
	foundDataPointDim = false;
}

void Cluster::setNumberOfClusters(int numberOfClusters)
{
	this->numberOfClusters = numberOfClusters;
	return;
}

int Cluster::getNumberOfClusters()
{
	return numberOfClusters;
}

void Cluster::initCentroids()
{
	int step = inputData.size()/numberOfClusters;
	centroids.clear();
	centroids.resize(numberOfClusters);
	for(int i = 0; i < numberOfClusters; i++){
		for (int j=0;j<inputData[i*step].size();j++){
			centroids[i].push_back(inputData[i*step][j]);
		}
	}

	return;
}

float Cluster::getEuclideanDistance(vector<float> sample, vector<float> centroid){
	float euclideanDistance = 0.0;
	for(int i = 0; i < dataPointDim; i++){
		euclideanDistance += pow(centroid[i] - sample[i], 2);
	}

	return sqrt(euclideanDistance);
}

int Cluster::prediccion_basada_en_Distancia(int User,int Item) {
	int ans=0;
	float may=0;
	for (int j=0;j<=10;j++){
		float count=0;
		for (int i=0;i<assignedClusters.size();i++){
			if(assignedClusters[i]==assignedClusters[User]){
				if(M[i].find(Item)!=M[i].end()){
					if(M[i][Item]==j){
						count+=1.0/getEuclideanDistance(inputData[User],inputData[i]);
					}
				}
			}
		}
		if(count>may){
			may=count;
			ans=j;
		}
	}
	return ans;
}

void Cluster::assignClusters() {
	vector<int> assignedClustersPrev = assignedClusters;
	int iteracion=0;
	bool areEquals=false;
	do
	{
		cout << "it -> " <<  iteracion++ << endl;
		assignedClustersPrev = assignedClusters;
		for(int i = 0; i < inputData.size(); i++)
		{
			float minDistance = numeric_limits<float>::max();
			for(int j = 0; j < numberOfClusters; j++)
			{
				float euclideanDistance = getEuclideanDistance(inputData[i], centroids[j]);
				if(euclideanDistance < minDistance)
				{
					assignedClusters[i] = j;
					minDistance = euclideanDistance;
				}
			}
		}

		updateCentroids();
		areEquals=true;
		int errors=0;
		for (int i=0;i<assignedClustersPrev.size();i++){
			if(assignedClustersPrev[i]!=assignedClusters[i]){
				areEquals=false;
				errors++;
			}
		}
		cout << "errors: " << errors << endl;
	} while(!areEquals);

	return;
}

void Cluster::updateCentroids()
{
	for(int i = 0; i < numberOfClusters; i++)
	{
		int clusterPointsCount = 0;
		vector<vector<float> > clusterPoints;

		for(int j = 0; j < assignedClusters.size(); j++)
		{
			if(assignedClusters[j] == i)
			{
				++clusterPointsCount;
				clusterPoints.push_back(inputData[j]);
			}
		}

		for(int j = 0; j < dataPointDim; j++)
		{
			float sum = 0.0;
			int total=0;
			for(int k = 0; k < clusterPoints.size(); k++){
				if(clusterPoints[k][j]>0){
					sum += clusterPoints[k][j];
					total++;
				}
			}
			centroids[i][j] = sum/total;
		}
	}

	return;
}

const int nrows=54157;
vector<vector<int> > V;
vector<string>BooksData;
vector<int> Users;

float Cluster::prediccionCompletaComp(string file,string original){
	ifstream in;
	in.open(original.c_str(),ios::in);
	map<int,vector<int> > Original;
	for (int i=0;i<97;i++){
		int User,Rank;
		in >> User;
		for (int j=0;j<dataPointDim;j++){
			in >> Rank;
			Original[User].push_back(Rank);
		}
	}
	in.close();
	file=file.substr(0,file.size()-4);
	file.push_back(getNumberOfClusters()+'0');
	file+="clusters.csv";
	ofstream of;
	of.open(file.c_str(),ios::out);
	of << "Usuarios\\Libros";
	for (int j=0;j<dataPointDim;j++){
		of << ";" << BooksData[j];
	}
	of << endl;
	float error=0;
	int total=0;
	for (int i=0;i<Users.size();i++){
		of << Users[i];
		for (int j=0;j<dataPointDim;j++){
			if(M[Users[i]].find(j)!=M[Users[i]].end()){
				of << ";" << M[Users[i]][j] ;
			} else {
				int prediccion= prediccion_basada_en_Distancia(i, j);
				total++;
				float errorU=Original[Users[i]][j]-prediccion;
				if(errorU<0){
					errorU*=-1;
				}
				error+=errorU;
				if(prediccion!=0){
					of << ";" << prediccion;
				} else {
					of << ";-";
				}
			}
		}
		of << endl;
	}
	of.close();
	error/=total;
	return error;
}

void Cluster::prediccionCompleta(string file){
	file=file.substr(0,file.size()-4);
	file.push_back(getNumberOfClusters()+'0');
	file+="clusters.csv";
	ofstream of;
	of.open(file.c_str(),ios::out);
	of << "Usuarios\\Libros";
	for (int j=0;j<dataPointDim;j++){
		of << ";" << BooksData[j];
	}
	of << endl;
	int total=0;
	for (int i=0;i<Users.size();i++){
		of << Users[i];
		for (int j=0;j<dataPointDim;j++){
			if(M[Users[i]].find(j)!=M[Users[i]].end()){
				of << ";" << M[Users[i]][j] ;
			} else {
				int prediccion= prediccion_basada_en_Distancia(i, j);
				if(prediccion!=0){
					of << ";" << prediccion;
				} else {
					of << ";-";
				}
			}
		}
		of << endl;
	}
	of.close();
	return;
}

void Cluster::Imprimir(string file){
	file=file.substr(0,file.size()-4);
	file.push_back(getNumberOfClusters()+'0');
	file+="original.csv";
	ofstream of;
	of.open(file.c_str(),ios::out);
	of << "Usuarios\\Libros";
	for (int j=0;j<dataPointDim;j++){
		of << ";" << BooksData[j];
	}
	of << endl;
	int total=0;
	for (int i=0;i<Users.size();i++){
		of << Users[i];
		for (int j=0;j<dataPointDim;j++){
			if(M[Users[i]].find(j)!=M[Users[i]].end()){
				of << ";" << M[Users[i]][j] ;
			} else {
				of << ";-";
			}
		}
		of << endl;
	}
	of.close();
	return;
}

void Cluster::process(string file)
{
	// Opening the input file for reading input data
	//ifstream inputFile;
	FILE *fp;
	fp= fopen(file.c_str(),"r");
	//inputFile.open(argv[1]);

	//Organizando ratings de libros
	int idBook=0;
	for (int row=0;row<nrows;row++){
		char bookC[15];
		char userC[15];
		char ratingC[3];
		//int rating=stoi(r.substr(1,r.size()-1));
		int user,rating;//=stoi(u.substr(1,u.size()-1));
		float frating;
		fscanf(fp,"%d %s %f",&user, bookC, &frating);
		int timestamp;frating*=2;fscanf(fp,"%d", &timestamp);//<-Solo si existe Time Stamp 
		rating=frating;
		string book(bookC);//,u(userC),r(ratingC);
		if(rating){
			if(Books.find(book)==Books.end()){
				Books[book]=make_pair(idBook++,1);
				BooksData.push_back(book);
			} else {
				Books[book].second++;
			}
			M[user][Books[book].first]=rating;
		}
	}
	fclose(fp);
	printf("Data importada\n");
	int nUsers=M.size();
	V.resize(nUsers);
	inputData.resize(nUsers);
	dataPointDim=Books.size();
	cout << "Users: " << nUsers << endl;
	cout << "Books: " << dataPointDim << endl;
	Users.resize(nUsers);
	int idUser=0;
	//Normalizando los datos con Gauss
	for (auto it=M.begin();idUser<nUsers && it!=M.end();it++){
		Users[idUser]=it->first;
		inputData[idUser].resize(dataPointDim);
		float total=0;
		for (int i=0;i<dataPointDim;i++){
			auto rating=(it->second).find(i);
			if(rating!=(it->second).end()){
				inputData[idUser][i]=rating->second;
				total+=rating->second;
			} else {
				inputData[idUser][i]=0;//-1;
				V[idUser].push_back(i);
			}
		}
		total/=dataPointDim;
		float sumPow=0;
		for (int i=0;i<dataPointDim;i++){
			sumPow+=pow(inputData[idUser][i]-total,2);
		}
		sumPow=sqrt(sumPow);
		for (int i=0;i<dataPointDim;i++){
			inputData[idUser][i]=(inputData[idUser][i]-total)/sumPow;
		}
		idUser++;
	}
	printf("Data Normalizada\n");
	assignedClusters.resize(inputData.size(), 1);

	initCentroids();
	printf("Centroides Inicializados\n");
	assignClusters();
	printf("Clusters Asignados\n");
	map<int,int> ClusterSize;
	for (int i=0;i<assignedClusters.size();i++){
		if(ClusterSize.find(assignedClusters[i])!=ClusterSize.end()){
			ClusterSize[assignedClusters[i]]++;
		} else {
			ClusterSize[assignedClusters[i]]=1;
		}
	}
	cout << "Distribución de Clusters" << endl;
	for (auto it=ClusterSize.begin();it!=ClusterSize.end();it++){
		cout << it->first << " = " << it->second << endl;
	}
	//cout << "Error Total: " << prediccionCompletaComp(file,"OriginalData.dat") << endl;
	prediccionCompleta(file);
	//Imprimir(file);
	return;
}
