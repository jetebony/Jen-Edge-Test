pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
		sh 'g++ test.cpp -o test'
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
