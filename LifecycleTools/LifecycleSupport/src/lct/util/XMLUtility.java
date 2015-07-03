package lct.util;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Text;
import org.xml.sax.SAXException;

public class XMLUtility {
	private static DocumentBuilderFactory s_documentBuilderFactory;
	private static TransformerFactory s_transformerFactory;
	
	static {
		s_documentBuilderFactory = DocumentBuilderFactory.newInstance();
		
		s_transformerFactory = TransformerFactory.newInstance();
		s_transformerFactory.setAttribute("indent-number", 4);
	}
	
	public static Document createDocument() throws ParserConfigurationException {
		DocumentBuilder documentBuilder = s_documentBuilderFactory.newDocumentBuilder();
		Document document = documentBuilder.newDocument();
		return document;
	}	
	
	public static void storeDocument(Document document, String filePath) throws UnsupportedEncodingException, FileNotFoundException, TransformerException {
		DOMSource source = new DOMSource(document);
		
		File file = new File(filePath);
		OutputStreamWriter outputStreamWriter = new OutputStreamWriter(new BufferedOutputStream(new FileOutputStream(file)), "UTF-8");
		StreamResult streamResult = new StreamResult(outputStreamWriter);
					
		Transformer transformer = s_transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.transform(source, streamResult);
	}
	
	public static Document loadDocument(String filePath) throws ParserConfigurationException, SAXException, IOException {
		File file = new File(filePath);
		
		DocumentBuilder documentBuilder = s_documentBuilderFactory.newDocumentBuilder();
		Document document = documentBuilder.parse(file);
		return document;
	}
	
	public static Element writeStringElement(Document document, String name, String value, Element parentElement) {
		Element element = document.createElement(name);
		if (value == null) {
			value = "null";
		}
		Text text = document.createTextNode(value);
		element.appendChild(text);
		parentElement.appendChild(element);
		return element;
	}
	
	public static String readStringElement(Element parentElement, String name, int index) {
		Element element = (Element)parentElement.getElementsByTagName(name).item(index);
		String text = element.getTextContent();
		if (text == "null") {
			text = null;
		}
		return text;
	}
	
	public static Element writeBooleanElement(Document document, String name, boolean value, Element parentElement) {
		Element element = document.createElement(name);
		Text text = document.createTextNode(Boolean.toString(value));
		element.appendChild(text);
		parentElement.appendChild(element);
		return element;
	}
	
	public static boolean readBooleanElement(Element parentElement, String name, int index) {
		Element element = (Element)parentElement.getElementsByTagName(name).item(index);
		String text = element.getTextContent();
		boolean value = Boolean.parseBoolean(text);
		return value;
	}
	
	public static Element writeIntegerElement(Document document, String name, int value, Element parentElement) {
		Element element = document.createElement(name);
		Text text = document.createTextNode(Integer.toString(value));
		element.appendChild(text);
		parentElement.appendChild(element);
		return element;
	}
	
	public static int readIntegerElement(Element parentElement, String name, int index) {
		Element element = (Element)parentElement.getElementsByTagName(name).item(index);
		String text = element.getTextContent();
		int value = Integer.parseInt(text);
		return value;
	}
	
	public static Element writeFloatElement(Document document, String name, float value, Element parentElement) {
		Element element = document.createElement(name);
		Text text = document.createTextNode(Float.toString(value));
		element.appendChild(text);
		parentElement.appendChild(element);
		return element;
	}
	
	public static float readFloatElement(Element parentElement, String name, int index) {
		Element element = (Element)parentElement.getElementsByTagName(name).item(index);
		String text = element.getTextContent();
		float value = Float.parseFloat(text);
		return value;
	}
}
