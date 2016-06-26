-- phpMyAdmin SQL Dump
-- version 
-- http://www.phpmyadmin.net
--
-- Хост: 
-- Время создания: Июн 26 2016 г., 19:45
-- Версия сервера: 5.6.25-73.1
-- Версия PHP: 5.6.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `??????????`
--

-- --------------------------------------------------------

--
-- Структура таблицы `sav_controllers`
--
-- Создание: Июн 24 2016 г., 10:00
--

DROP TABLE IF EXISTS `sav_controllers`;
CREATE TABLE IF NOT EXISTS `sav_controllers` (
  `npp` decimal(5,0) NOT NULL,
  `name` varchar(32) NOT NULL,
  `text` varchar(256) NOT NULL,
  `enable` decimal(1,0) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `sav_plant_c04`
--
-- Создание: Июн 24 2016 г., 10:00
--

DROP TABLE IF EXISTS `sav_plant_c04`;
CREATE TABLE IF NOT EXISTS `sav_plant_c04` (
  `name` varchar(32) NOT NULL,
  `id` decimal(4,0) NOT NULL,
  `dt` datetime NOT NULL,
  `count` decimal(5,0) NOT NULL,
  `soil` decimal(5,0) NOT NULL,
  `vcc` decimal(5,0) NOT NULL,
  `t` decimal(5,0) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `sav_plant_sprav04`
--
-- Создание: Июн 26 2016 г., 08:31
--

DROP TABLE IF EXISTS `sav_plant_sprav04`;
CREATE TABLE IF NOT EXISTS `sav_plant_sprav04` (
  `name` varchar(32) NOT NULL,
  `id` decimal(4,0) NOT NULL,
  `text` varchar(256) NOT NULL,
  `enable` decimal(1,0) NOT NULL,
  `k` decimal(5,0) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `sav_controllers`
--
ALTER TABLE `sav_controllers`
  ADD UNIQUE KEY `npp` (`npp`);

--
-- Индексы таблицы `sav_plant_c04`
--
ALTER TABLE `sav_plant_c04`
  ADD KEY `sav_plant_c04_i1` (`id`), ADD KEY `sav_plant_c04_i2` (`dt`), ADD KEY `sav_plant_c04_i0` (`name`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
