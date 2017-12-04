class KMeans
{
// private members
	// Dimension of each data-point/sample/observation from the input data
	int dataPointDim;

	// Number of clusters to be created - provided by the user
	int numberOfClusters;

	// Flag to check if the dimension of each data-point is found or not
	bool foundDataPointDim;

	// Input data provided by the user
	std::vector<std::vector<float> > inputData;

	// Centroids of cluster
	std::vector<std::vector<float> > centroids;

	// Array repesenting which data-point is assigned to which cluster
	std::vector<int> assignedClusters;

	// To initialize the centroids of the clusters to different data-points from the input data
	// using the number of data-points and clusers provided by the user
	void initCentroids();

	// To Calculate the Euclidean distance between centroid vector and data-point vector
	float getEuclideanDistance(std::vector<float> sample, std::vector<float> centroid);

	// To assign the data-point to one of the clusters 
	// based on minimum distance between centroid and the data-point
	void assignClusters();

	// To calculate the means as centroids of the new clusters
	void updateCentroids();

	// Destructor
	~KMeans();

// public members
public:
	// Constructor
	KMeans();

	// To start k-means clustering for input data
	void process(string file);

	// Setter example
	void setNumberOfClusters(int numberOfClusters);
	
	// Getter example
	int getNumberOfClusters();
	int prediccion_basada_en_Distancia(int User,int Item);
	float prediccionCompleta(string file);
};

map<int,map<int,int> > M;
map<string,pair<int,int> > Books;

KMeans::KMeans()
{
	dataPointDim = 0;
	numberOfClusters = 0;
	foundDataPointDim = false;
}

void KMeans::setNumberOfClusters(int numberOfClusters)
{
	// Use of 'this' keyword
	this->numberOfClusters = numberOfClusters;
	return;
}

int KMeans::getNumberOfClusters()
{
	return numberOfClusters;
}

void KMeans::initCentroids()
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

float KMeans::getEuclideanDistance(std::vector<float> sample, std::vector<float> centroid)
{
	float euclideanDistance = 0.0;
	for(int i = 0; i < dataPointDim; i++){
		euclideanDistance += pow(centroid[i] - sample[i], 2);
	}

	return sqrt(euclideanDistance);
}

int KMeans::prediccion_basada_en_Distancia(int User,int Item){
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

void KMeans::assignClusters()
{
	std::vector<int> assignedClustersPrev = assignedClusters;
	int iteracion=0;
	bool areEquals=false;
	do
	{
		cout << "it -> " <<  iteracion++ << endl;
		assignedClustersPrev = assignedClusters;
		for(int i = 0; i < inputData.size(); i++)
		{
			float minDistance = std::numeric_limits<float>::max();
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
	// Keep updating until assignedClusters is not updating
	} while(!areEquals);

	return;
}

void KMeans::updateCentroids()
{
	for(int i = 0; i < numberOfClusters; i++)
	{
		int clusterPointsCount = 0;
		vector<vector<float> > clusterPoints;

		// Counting the number of data-points assigned to current cluster
		for(int j = 0; j < assignedClusters.size(); j++)
		{
			if(assignedClusters[j] == i)
			{
				++clusterPointsCount;
				clusterPoints.push_back(inputData[j]);
			}
		}

		// Calculating the mean as new centroid
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

float KMeans::prediccionCompleta(string file){
	ifstream in;
	in.open("OriginalData.dat",ios::in);
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

void KMeans::process(string file)
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
		fscanf(fp,"%d %s %d",&user, bookC, &rating);
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
	// Setting the size of assignedClusters vector same as number of input data-points
	assignedClusters.resize(inputData.size(), 1);

	// Copying input data to output if number of clusters expected is same as size of input data
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
	cout << "Error Total: " << prediccionCompleta(file) << endl;
	return;
}
