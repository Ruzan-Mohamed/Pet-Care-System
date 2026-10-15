-- =====================================================================
-- PAWCARE PET CARE SYSTEM - DATABASE SCHEMA
-- Target RDBMS: MySQL 8.0+ / MariaDB 10.4+
-- Database: pawcare_db
-- =====================================================================

CREATE DATABASE IF NOT EXISTS `pawcare_db`
CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

USE `pawcare_db`;

-- Drop tables in reverse order of foreign key dependencies if re-running
SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS `payments`;
DROP TABLE IF EXISTS `medical_records`;
DROP TABLE IF EXISTS `appointments`;
DROP TABLE IF EXISTS `pets`;
DROP TABLE IF EXISTS `veterinarians`;
DROP TABLE IF EXISTS `owners`;
SET FOREIGN_KEY_CHECKS = 1;

-- ---------------------------------------------------------------------
-- 1. OWNERS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `owners` (
    `owner_id` INT AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(100) NOT NULL,
    `telephone` VARCHAR(20) NOT NULL,
    `email` VARCHAR(120) NULL,
    `address` VARCHAR(255) NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `chk_owner_name` CHECK (CHAR_LENGTH(TRIM(`name`)) > 0),
    CONSTRAINT `chk_owner_tel` CHECK (CHAR_LENGTH(TRIM(`telephone`)) >= 7)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- 2. VETERINARIANS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `veterinarians` (
    `veterinarian_id` INT AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(100) NOT NULL,
    `telephone` VARCHAR(20) NULL,
    `specialization` VARCHAR(100) NOT NULL DEFAULT 'General Veterinary',
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `chk_vet_name` CHECK (CHAR_LENGTH(TRIM(`name`)) > 0)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- 3. PETS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `pets` (
    `pet_id` INT AUTO_INCREMENT PRIMARY KEY,
    `owner_id` INT NOT NULL,
    `name` VARCHAR(100) NOT NULL,
    `pet_type` VARCHAR(50) NOT NULL,
    `breed` VARCHAR(100) NULL,
    `age` INT NOT NULL DEFAULT 0,
    `gender` ENUM('Male', 'Female', 'Unknown') NOT NULL DEFAULT 'Unknown',
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `fk_pets_owner` FOREIGN KEY (`owner_id`)
        REFERENCES `owners` (`owner_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    CONSTRAINT `chk_pet_age` CHECK (`age` >= 0 AND `age` <= 100),
    CONSTRAINT `chk_pet_name` CHECK (CHAR_LENGTH(TRIM(`name`)) > 0)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- 4. APPOINTMENTS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `appointments` (
    `appointment_id` INT AUTO_INCREMENT PRIMARY KEY,
    `pet_id` INT NOT NULL,
    `veterinarian_id` INT NOT NULL,
    `appointment_date` DATE NOT NULL,
    `appointment_time` TIME NOT NULL,
    `reason` VARCHAR(255) NOT NULL,
    `status` ENUM('BOOKED', 'CONFIRMED', 'COMPLETED', 'CANCELLED') NOT NULL DEFAULT 'BOOKED',
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `fk_appts_pet` FOREIGN KEY (`pet_id`)
        REFERENCES `pets` (`pet_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    CONSTRAINT `fk_appts_vet` FOREIGN KEY (`veterinarian_id`)
        REFERENCES `veterinarians` (`veterinarian_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    INDEX `idx_vet_schedule` (`veterinarian_id`, `appointment_date`, `appointment_time`),
    INDEX `idx_pet_appointments` (`pet_id`, `appointment_date`)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- 5. MEDICAL RECORDS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `medical_records` (
    `record_id` INT AUTO_INCREMENT PRIMARY KEY,
    `appointment_id` INT NOT NULL,
    `pet_id` INT NOT NULL,
    `diagnosis` TEXT NOT NULL,
    `treatment` TEXT NOT NULL,
    `vaccination` VARCHAR(150) NULL,
    `notes` TEXT NULL,
    `record_date` DATE NOT NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `fk_medrec_appt` FOREIGN KEY (`appointment_id`)
        REFERENCES `appointments` (`appointment_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    CONSTRAINT `fk_medrec_pet` FOREIGN KEY (`pet_id`)
        REFERENCES `pets` (`pet_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    INDEX `idx_medrec_pet` (`pet_id`),
    INDEX `idx_medrec_appt` (`appointment_id`)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- 6. PAYMENTS TABLE
-- ---------------------------------------------------------------------
CREATE TABLE `payments` (
    `payment_id` INT AUTO_INCREMENT PRIMARY KEY,
    `appointment_id` INT NOT NULL,
    `consultation_fee` DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    `treatment_fee` DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    `other_fee` DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    `total_amount` DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    `payment_method` ENUM('Cash', 'Card', 'Bank Transfer', 'Online') NOT NULL DEFAULT 'Cash',
    `payment_status` ENUM('PENDING', 'PAID', 'FAILED', 'CANCELLED', 'REFUNDED') NOT NULL DEFAULT 'PENDING',
    `payment_date` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT `fk_pay_appt` FOREIGN KEY (`appointment_id`)
        REFERENCES `appointments` (`appointment_id`)
        ON UPDATE CASCADE
        ON DELETE RESTRICT,
    CONSTRAINT `chk_fees_nonnegative` CHECK (
        `consultation_fee` >= 0.00 AND
        `treatment_fee` >= 0.00 AND
        `other_fee` >= 0.00 AND
        `total_amount` >= 0.00
    ),
    INDEX `idx_pay_appt` (`appointment_id`)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- SAMPLE SEED DATA
-- ---------------------------------------------------------------------
INSERT INTO `owners` (`owner_id`, `name`, `telephone`, `email`, `address`) VALUES
(1, 'Kasun Perera', '0712345678', 'kasun@example.com', 'Colombo, Sri Lanka'),
(2, 'Nimal Fernando', '0779876543', 'nimal@example.com', 'Kandy, Sri Lanka');

INSERT INTO `veterinarians` (`veterinarian_id`, `name`, `telephone`, `specialization`) VALUES
(1, 'Dr. Silva', '0112345678', 'General Veterinary'),
(2, 'Dr. Samantha', '0119876543', 'Feline Surgery');

INSERT INTO `pets` (`pet_id`, `owner_id`, `name`, `pet_type`, `breed`, `age`, `gender`) VALUES
(1, 1, 'Max', 'Dog', 'Labrador', 3, 'Male'),
(2, 1, 'Luna', 'Cat', 'Persian', 2, 'Female');

INSERT INTO `appointments` (`appointment_id`, `pet_id`, `veterinarian_id`, `appointment_date`, `appointment_time`, `reason`, `status`) VALUES
(1, 1, 1, '2026-10-15', '09:00:00', 'General Checkup', 'BOOKED');

INSERT INTO `medical_records` (`record_id`, `appointment_id`, `pet_id`, `diagnosis`, `treatment`, `vaccination`, `notes`, `record_date`) VALUES
(1, 1, 1, 'Skin Allergy', 'Prescribed medication', 'Rabies', 'Follow-up in 2 weeks', '2026-10-15');

INSERT INTO `payments` (`payment_id`, `appointment_id`, `consultation_fee`, `treatment_fee`, `other_fee`, `total_amount`, `payment_method`, `payment_status`) VALUES
(1, 1, 1200.00, 1000.00, 0.00, 2200.00, 'Cash', 'PAID');
