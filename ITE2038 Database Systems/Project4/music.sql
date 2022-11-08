-- MariaDB dump 10.17  Distrib 10.5.6-MariaDB, for Win64 (AMD64)
--
-- Host: localhost    Database: music
-- ------------------------------------------------------
-- Server version	10.5.6-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `music`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `music` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `music`;

--
-- Table structure for table `admin`
--

DROP TABLE IF EXISTS `admin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `admin` (
  `AdminID` varchar(15) NOT NULL,
  `AdminPW` varchar(15) NOT NULL,
  PRIMARY KEY (`AdminID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `admin`
--

LOCK TABLES `admin` WRITE;
/*!40000 ALTER TABLE `admin` DISABLE KEYS */;
INSERT INTO `admin` VALUES ('coral2cola','1234'),('love','1234');
/*!40000 ALTER TABLE `admin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `album`
--

DROP TABLE IF EXISTS `album`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `album` (
  `albumNum` int(11) NOT NULL,
  `albumName` varchar(30) NOT NULL,
  `releaseDate` date DEFAULT NULL,
  `mgr_ID` varchar(15) DEFAULT NULL,
  `artistNum` int(11) NOT NULL,
  PRIMARY KEY (`albumNum`),
  KEY `FK_album_artist` (`artistNum`),
  KEY `FK_album_admin` (`mgr_ID`),
  CONSTRAINT `FK_album_admin` FOREIGN KEY (`mgr_ID`) REFERENCES `admin` (`AdminID`),
  CONSTRAINT `FK_album_artist` FOREIGN KEY (`artistNum`) REFERENCES `artist` (`artistNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `album`
--

LOCK TABLES `album` WRITE;
/*!40000 ALTER TABLE `album` DISABLE KEYS */;
INSERT INTO `album` VALUES (1,'Propose','2017-07-08','coral2cola',1),(2,'It\'s Okay, Dear','2013-04-02','coral2cola',1),(3,'Stand','2019-05-30','love',1),(5,'NAN CHUN','2020-05-10','love',2),(6,'Eternity','2018-12-10','coral2cola',1),(7,'Heroine','2018-01-18','love',3),(8,'WARNING','2018-09-04','coral2cola',3),(9,'Gashina','2017-08-22','love',3),(10,'Pallette','2017-04-21','coral2cola',4),(11,'VVS','2020-12-05','love',6);
/*!40000 ALTER TABLE `album` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `artist`
--

DROP TABLE IF EXISTS `artist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `artist` (
  `artistNum` int(11) NOT NULL,
  `artistName` varchar(15) NOT NULL,
  `debut` year(4) DEFAULT NULL,
  `year` year(4) DEFAULT NULL,
  `nation` varchar(15) DEFAULT NULL,
  `mgr_ID` varchar(15) NOT NULL,
  PRIMARY KEY (`artistNum`),
  KEY `FK_artist_admin` (`mgr_ID`),
  CONSTRAINT `FK_artist_admin` FOREIGN KEY (`mgr_ID`) REFERENCES `admin` (`AdminID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `artist`
--

LOCK TABLES `artist` WRITE;
/*!40000 ALTER TABLE `artist` DISABLE KEYS */;
INSERT INTO `artist` VALUES (1,'sunwoojunga',1985,2020,'Korea','love'),(2,'sesoneon',2017,2020,'Korea','coral2cola'),(3,'SUNMI',2007,2020,'Korea','love'),(4,'IU',2008,2020,'Korea','coral2cola'),(6,'Mushvenom',2019,2020,'Korea','coral2cola');
/*!40000 ALTER TABLE `artist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `music`
--

DROP TABLE IF EXISTS `music`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `music` (
  `musicNum` int(11) NOT NULL,
  `musicName` varchar(30) NOT NULL,
  `trackNum` int(11) DEFAULT NULL,
  `releaseDate` date DEFAULT NULL,
  `mgr_ID` varchar(15) NOT NULL,
  `albumNum` int(11) NOT NULL,
  `artistNum` int(11) NOT NULL,
  PRIMARY KEY (`musicNum`),
  KEY `FK_music_admin` (`mgr_ID`),
  KEY `FK_music_artist` (`artistNum`),
  KEY `FK_music_album` (`albumNum`),
  CONSTRAINT `FK_music_admin` FOREIGN KEY (`mgr_ID`) REFERENCES `admin` (`AdminID`),
  CONSTRAINT `FK_music_album` FOREIGN KEY (`albumNum`) REFERENCES `album` (`albumNum`),
  CONSTRAINT `FK_music_artist` FOREIGN KEY (`artistNum`) REFERENCES `artist` (`artistNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `music`
--

LOCK TABLES `music` WRITE;
/*!40000 ALTER TABLE `music` DISABLE KEYS */;
INSERT INTO `music` VALUES (1,'Propose',1,'2017-07-08','coral2cola',1,1),(2,'Purple Daddy',4,'2013-04-02','coral2cola',2,1),(3,'Beap Sea',2,'2013-04-02','coral2cola',2,1),(8,'NAN CHUN',1,'2020-05-10','coral2cola',5,2),(9,'Eternity',1,'2018-12-10','coral2cola',6,1),(10,'Heroine',1,'2018-01-18','love',7,3),(11,'Siren',1,'2018-09-04','coral2cola',8,3),(12,'SAM SAM',1,'2019-05-30','love',3,1),(13,'Superhero',1,'2019-05-30','love',3,1),(14,'Gashina',1,'2017-08-22','love',9,3),(15,'Pallette',1,'2017-04-21','coral2cola',10,4),(16,'Ready',3,'2019-05-30','coral2cola',3,1),(17,'VVS',1,'2020-12-05','love',11,6);
/*!40000 ALTER TABLE `music` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `playlist`
--

DROP TABLE IF EXISTS `playlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `playlist` (
  `index` int(11) NOT NULL,
  `userID` varchar(45) NOT NULL,
  `musicNum` int(11) NOT NULL,
  PRIMARY KEY (`userID`,`musicNum`),
  KEY `FK_playlist_music` (`musicNum`),
  CONSTRAINT `FK_playlist_music` FOREIGN KEY (`musicNum`) REFERENCES `music` (`musicNum`),
  CONSTRAINT `FK_playlist_user` FOREIGN KEY (`userID`) REFERENCES `user` (`userID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `playlist`
--

LOCK TABLES `playlist` WRITE;
/*!40000 ALTER TABLE `playlist` DISABLE KEYS */;
INSERT INTO `playlist` VALUES (1,'3zu',1),(2,'3zu',3),(1,'3zu',10),(1,'moonshort',12);
/*!40000 ALTER TABLE `playlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `userID` varchar(15) NOT NULL,
  `userPW` varchar(15) NOT NULL,
  `userAge` int(11) DEFAULT NULL,
  `mgr_ID` varchar(15) NOT NULL,
  PRIMARY KEY (`userID`),
  KEY `FK_user_admin` (`mgr_ID`),
  CONSTRAINT `FK_user_admin` FOREIGN KEY (`mgr_ID`) REFERENCES `admin` (`AdminID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('3zu','1234',21,'coral2cola'),('moonshort','1111',19,'love'),('raffine','1234',22,'love');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-12-06 12:26:00
