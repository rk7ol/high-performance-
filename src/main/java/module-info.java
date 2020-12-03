module HPHC {
    requires javafx.graphics;
    requires javafx.fxml;
    requires org.apache.avro;
    requires jzy3d.api;
    requires javafx.controls;
    requires kafka.clients;
    requires jzy3d.javafx;
    requires javafx.swing;


    requires com.fasterxml.jackson.core;


    exports HighPerformanceHC.controllers;

    exports HighPerformanceHC;


}