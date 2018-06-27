pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
		sh '/integra/opt/bin/g++ test.cpp -o test'
            }
        }
	stage('Test') {
            steps {
		sh '''
		./test
		'''
            }
	}
    }
}
