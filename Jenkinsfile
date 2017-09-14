pipeline {
    agent any
    stages {
        stage('build') {
            steps {
		echo "Building...."
                sh 'make clean'
		sh 'make arm'
            }
        }
	stage('test)' {
            steps {
		echo "Testing...."
		sh 'make test'
	    }
	}
    }
}
